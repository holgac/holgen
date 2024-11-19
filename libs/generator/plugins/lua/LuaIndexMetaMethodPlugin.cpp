#include "LuaIndexMetaMethodPlugin.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

void LuaIndexMetaMethodPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua))
      continue;
    Process(cls);
  }
}

void LuaIndexMetaMethodPlugin::Process(Class &cls) {
  auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(R"R(HOLGEN_WARN("Unexpected lua field: {}.{{}}", key);)R",
                             cls.mStruct->mName);
  stringSwitcherElseCase.Add("return 0;");
  StringSwitcher switcher("key", std::move(stringSwitcherElseCase));

  GenerateIndexMetaMethodForFields(cls, switcher);
  GenerateIndexMetaMethodForExposedMethods(cls, switcher);
  if (cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    GenerateIndexMetaMethodForFuncTable(cls, switcher);
  }

  if (!switcher.IsEmpty()) {
    method.mBody.Add("const char *key = lua_tostring(luaState, -1);");
    method.mBody.Add(std::move(switcher.Generate()));
    method.mBody.Line() << "return 1;";
  } else {
    method.mBody.Line() << "return 0;";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForFuncTable(Class &cls,
                                                                   StringSwitcher &switcher) {
  switcher.AddCase(St::LuaTable_TableFieldInIndexMethod, [&](CodeBlock &switchBlock) {
    GenerateInstanceGetter(cls, switchBlock, -2, "instance");
    switchBlock.Add("{}::{}<false>(instance->{}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                    Naming().FieldNameInCpp(St::LuaTable_TableField));
  });
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForFields(Class &cls,
                                                                StringSwitcher &switcher) {
  for (auto &field: cls.mFields) {
    // TODO: handle variant type (should call setter instead of direct assignment)
    if (!field.mField || field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Lua) ||
        field.mField->mType.mName == St::UserData)
      continue;
    auto fieldClass = mProject.GetClass(field.mType.mName);
    if (fieldClass && fieldClass->mStruct &&
        (fieldClass->mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
         fieldClass->mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua)))
      continue;
    bool isRef = field.mField->mType.mName == "Ref";
    switcher.AddCase(Naming().FieldNameInLua(*field.mField), [&](CodeBlock &switchBlock) {
      GenerateIndexForField(cls, field, switchBlock);
    });
    if (isRef && field.mType.mType != PassByType::Pointer) {
      auto underlyingStruct =
          mProject.mProject.GetStruct(field.mField->mType.mTemplateParameters.front().mName);
      if (underlyingStruct->GetAnnotation(Annotations::Managed)) {
        switcher.AddCase(Naming().FieldNameInLua(*field.mField, true), [&](CodeBlock &switchBlock) {
          GenerateInstanceGetter(cls, switchBlock, -2, "instance");
          switchBlock.Add("{}::{}<false>({}::{}(instance->{}), luaState);", St::LuaHelper,
                          St::LuaHelper_Push, field.mField->mType.mTemplateParameters[0].mName,
                          St::ManagedObject_Getter, field.mName);
        });
      }
    }
  }
}

void LuaIndexMetaMethodPlugin::GenerateMethodCaller(Class &cls, const ClassMethod &method) {
  auto methodCaller = ClassMethod{Naming().LuaMethodCaller(method), Type{"int"},
                                  Visibility::Private, Constness::NotConst, Staticness::Static};
  methodCaller.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  bool isLuaFuncTable = false;
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    isLuaFuncTable = true;
  }
  bool isLuaFunc = isLuaFuncTable;
  if (method.mFunction && method.mFunction->GetAnnotation(Annotations::LuaFunc)) {
    isLuaFunc = true;
  }

  std::string callPrefix;
  if (method.mStaticness == Staticness::Static) {
    callPrefix = std::format("{}::", cls.mName);
  } else {
    GenerateInstanceGetter(cls, methodCaller.mBody,
                           -ptrdiff_t(method.mArguments.size()) - 1 + isLuaFunc, "instance");
    methodCaller.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!instance, 0, \"Calling {}.{} method with "
                           "an invalid lua proxy object!\");",
                           cls.mName, method.mName);
    callPrefix = std::format("instance->", cls.mName);
  }
  std::string funcArgs =
      GenerateReadExposedMethodArgsAndGetArgsString(cls, method, methodCaller.mBody, isLuaFunc);

  if (isLuaFunc) {
    if (funcArgs.empty()) {
      funcArgs = "luaState";
    } else {
      funcArgs = std::format("luaState, {}", funcArgs);
    }
  }

  if (method.mReturnType.mName != "void") {
    if (method.mReturnType.mType == PassByType::Reference) {
      methodCaller.mBody.Add("auto& result = {}{}({});", callPrefix, method.mName, funcArgs);
    } else {
      methodCaller.mBody.Add("auto result = {}{}({});", callPrefix, method.mName, funcArgs);
    }
    std::string accessor = ".";
    if (method.mReturnType.mType == PassByType::Pointer) {
      accessor = "->";
    }
    auto returnedClass = mProject.GetClass(method.mReturnType.mName);
    if (returnedClass && !returnedClass->mEnum) {
      bool pushMirror = method.mReturnType.mType == PassByType::Value;
      methodCaller.mBody.Add("{}::{}<{}>(result, luaState);", St::LuaHelper, St::LuaHelper_Push,
                             pushMirror ? "true" : "false");
    } else {
      bool shouldBeMirror = false;
      if (method.mReturnType.mType == PassByType::Value) {
        shouldBeMirror = true;
        if (TypeInfo::Get().CppIndexedContainers.contains(method.mReturnType.mName)) {
          auto &underlyingType = method.mReturnType.mTemplateParameters.front();
          if (underlyingType.mType == PassByType::Pointer) {
            shouldBeMirror = false;
          }
        }
      }
      methodCaller.mBody.Add("{}::{}<{}>(result, luaState);", St::LuaHelper, St::LuaHelper_Push,
                             shouldBeMirror ? "true" : "false");
    }
    methodCaller.mBody.Add("return 1;");
  } else {
    methodCaller.mBody.Add("{}{}({});", callPrefix, method.mName, funcArgs);
    methodCaller.mBody.Add("return 0;");
  }
  Validate().NewMethod(cls, methodCaller);
  cls.mMethods.push_back(std::move(methodCaller));
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForExposedMethods(Class &cls,
                                                                        StringSwitcher &switcher) {
  for (auto &exposedMethod: cls.mMethods) {
    if (!ShouldGenerateWrapper(exposedMethod))
      continue;
    GenerateMethodCaller(cls, exposedMethod);
    std::string methodSuffix;
    if (exposedMethod.mFunction &&
        exposedMethod.mFunction->GetMatchingAttribute(Annotations::Func,
                                                      Annotations::Func_OverloadSuffix)) {
      methodSuffix = exposedMethod.mFunction
                         ->GetMatchingAttribute(Annotations::Func, Annotations::Func_OverloadSuffix)
                         ->mValue.mName;
    }
    switcher.AddCase(exposedMethod.mName + methodSuffix, [&](CodeBlock &switchBlock) {
      // TODO: LuaHelper::Push should work with functions
      switchBlock.Add("lua_pushcfunction(luaState, {}::{});", cls.mName,
                      Naming().LuaMethodCaller(exposedMethod));
    });
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexForField(Class &cls, ClassField &field,
                                                     CodeBlock &switchBlock) {
  GenerateInstanceGetter(cls, switchBlock, -2, "instance");
  switchBlock.Add("HOLGEN_WARN_AND_RETURN_IF(!instance, 0, \"Requesting for {}.{} with "
                  "an invalid lua proxy object!\");",
                  cls.mName, field.mField ? field.mField->mName : field.mName);
  if (field.mField->mType.mName == St::Variant) {
    GenerateIndexForVariantField(cls, field, switchBlock);
  } else if (field.mField->mType.mName == St::Lua_CustomData) {
    GenerateIndexForRegistryData(field, switchBlock);
  } else {
    switchBlock.Add("{}::{}<false>(instance->{}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                    field.mName);
  }
}

std::string LuaIndexMetaMethodPlugin::GenerateReadExposedMethodArgsAndGetArgsString(
    const Class &cls, const ClassMethod &exposedMethod, CodeBlock &switchBlock, bool isLuaFunc) {
  std::stringstream funcArgs;
  size_t i = 0;
  for (auto &arg: exposedMethod.mArguments) {
    if (arg.mType.mName == "lua_State") {
      THROW_IF(i != 0 || !isLuaFunc, "Unexpected lua_State argument!");
      continue;
    }
    if (i != 0)
      funcArgs << ", ";
    ptrdiff_t stackIdx = ptrdiff_t(i) - ptrdiff_t(exposedMethod.mArguments.size()) + isLuaFunc;
    if (auto argClass = mProject.GetClass(arg.mType.mName)) {
      bool canBeMirror = !cls.HasVirtualMethods() && arg.mType.SupportsMirroring(mProject);
      bool canBeProxy = true;
      bool canBeNull = arg.mType.mType == PassByType::Pointer;
      if (arg.mType.mType != PassByType::Value && arg.mType.mConstness == Constness::NotConst) {
        canBeMirror = false;
      }
      if (argClass->mEnum) {
        canBeProxy = false;
      }
      if (canBeProxy && canBeMirror) {
        switchBlock.Add("{} arg{}Mirror;", arg.mType.mName, i);
        switchBlock.Add("{} *arg{};", arg.mType.mName, i);
        if (canBeNull) {
          switchBlock.Add("if (lua_isnil(luaState, {})) {{", stackIdx);
          switchBlock.Indent(1);
          switchBlock.Add("arg{} = nullptr;", i);
          switchBlock.Indent(-1);
          switchBlock.Add("}} else if (lua_getmetatable(luaState, {})) {{", stackIdx);
        } else {
          switchBlock.Add("if (lua_getmetatable(luaState, {})) {{", stackIdx);
        }
        switchBlock.Indent(1);
        switchBlock.Add("lua_pop(luaState, 1);");
        switchBlock.Add("arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadProxyObject, stackIdx);
        switchBlock.Indent(-1);
        switchBlock.Add("}} else {{");
        switchBlock.Indent(1);
        switchBlock.Add("arg{}Mirror = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadMirrorObject, stackIdx);
        switchBlock.Add("arg{0} = &arg{0}Mirror;", i);
        switchBlock.Indent(-1);
        switchBlock.Add("}}");
      } else if (canBeProxy) {
        switchBlock.Add("auto arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadProxyObject, stackIdx);
      } else if (canBeMirror) {
        switchBlock.Add("auto arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadMirrorObject, stackIdx);
      } else {
        THROW("Dont know how to pass {}.{} argument", exposedMethod.mName, arg.mName);
      }
      if (canBeProxy) {
        if (arg.mType.mType == PassByType::Pointer) {
          funcArgs << "arg" << i;
        } else {
          funcArgs << "*arg" << i;
        }
      } else {
        if (arg.mType.mType == PassByType::Pointer) {
          funcArgs << "&arg" << i;
        } else {
          funcArgs << "arg" << i;
        }
      }
    } else {
      auto sanitizedType = arg.mType;
      sanitizedType.mType = PassByType::Value;
      sanitizedType.mConstness = Constness::NotConst;
      switchBlock.Add("{}arg{};", sanitizedType.ToString(false), i);
      switchBlock.Add("{}::{}(arg{}, luaState, {});", St::LuaHelper, St::LuaHelper_Read, i,
                      stackIdx);
      funcArgs << "arg" << i;
    }
    ++i;
  }
  return funcArgs.str();
}

void LuaIndexMetaMethodPlugin::GenerateIndexForVariantField(Class &cls, ClassField &field,
                                                            CodeBlock &switchBlock) {
  auto enumField =
      cls.GetField(Naming().FieldNameInCpp(field.mField->GetAnnotation(Annotations::Variant)
                                               ->GetAttribute(Annotations::Variant_TypeField)
                                               ->mValue.mName));
  switchBlock.Add("switch (instance->{}.GetValue()) {{", enumField->mName);
  for (auto &otherCls: mProject.mClasses) {
    if (!otherCls.mStruct || !otherCls.mStruct->GetAnnotation(Annotations::Variant))
      continue;
    auto variantAnnotation = otherCls.mStruct->GetAnnotation(Annotations::Variant);
    if (variantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName !=
        enumField->mType.mName)
      continue;
    switchBlock.Add("case {}::{}:", enumField->mType.ToString(true),
                    variantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
    switchBlock.Indent(1);
    switchBlock.Add("{}::{}<false>(instance->{}(), luaState);", St::LuaHelper, St::LuaHelper_Push,
                    Naming().VariantGetterNameInCpp(*field.mField, *otherCls.mStruct));
    switchBlock.Add("break;");
    switchBlock.Indent(-1);
  }
  switchBlock.Add("default:");
  switchBlock.Indent(1);
  switchBlock.Add("lua_pushnil(luaState);");
  switchBlock.Indent(-1);
  switchBlock.Add("}}");
}

void LuaIndexMetaMethodPlugin::GenerateIndexForRegistryData(ClassField &field,
                                                            CodeBlock &switchBlock) {
  switchBlock.Add("lua_rawgeti(luaState, LUA_REGISTRYINDEX, instance->{});", field.mName);
}

void LuaIndexMetaMethodPlugin::GenerateInstanceGetter(Class &cls, CodeBlock &codeBlock, int index,
                                                      const std::string &outVarName = "instance") {
  codeBlock.Add("auto {} = {}::{}(luaState, {});", outVarName, cls.mName, St::Lua_ReadProxyObject,
                index);
}

bool LuaIndexMetaMethodPlugin::ShouldGenerateWrapper(const ClassMethod &method) const {
  if (!method.mExposeToLua)
    return false;
  if (method.mFunction &&
      method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_OnDataLoad))
    return false;
  return true;
}

} // namespace holgen
