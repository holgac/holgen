#include "LuaIndexMetaMethodPlugin.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

void LuaIndexMetaMethodPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || cls.mStruct->GetAnnotation(Annotations::NoLua))
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
  bool hasFields = !switcher.IsEmpty();
  GenerateIndexMetaMethodForExposedMethods(cls, switcher);

  if (!switcher.IsEmpty()) {
    if (hasFields) {
      method.mBody.Add("auto instance = {}::{}(luaState, -2);", cls.mName, St::Lua_ReadProxyObject);
    }
    method.mBody.Add("const char *key = lua_tostring(luaState, -1);");
    method.mBody.Add(std::move(switcher.Generate()));
    method.mBody.Line() << "return 1;";
  } else {
    method.mBody.Line() << "return 0;";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForFields(Class &cls,
                                                                StringSwitcher &switcher) {
  for (auto &field: cls.mFields) {
    // TODO: handle variant type (should call setter instead of direct assignment)
    if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua) ||
        field.mField->mType.mName == St::UserData)
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
          switchBlock.Add("{}::{}({}::{}(instance->{}), luaState, false);", St::LuaHelper,
                          St::LuaHelper_Push, field.mField->mType.mTemplateParameters[0].mName,
                          St::ManagedObject_Getter, field.mName);
        });
      }
    }
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForExposedMethods(Class &cls,
                                                                        StringSwitcher &switcher) {
  bool isLuaFuncTable = false;
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    isLuaFuncTable = true;
  }
  for (auto &exposedMethod: cls.mMethods) {
    if (!exposedMethod.mExposeToLua)
      continue;
    switcher.AddCase(exposedMethod.mName, [&, isLuaFuncTable](CodeBlock &switchBlock) {
      bool isLuaFunc = isLuaFuncTable;
      if (exposedMethod.mFunction && exposedMethod.mFunction->GetAnnotation(Annotations::LuaFunc)) {
        isLuaFunc = true;
      }

      // TODO: LuaHelper::Push should work with functions
      switchBlock.Add("lua_pushcfunction(luaState, [](lua_State *lsInner) {{");
      switchBlock.Indent(1);
      switchBlock.Add("auto instance = {}::{}(lsInner, {});", cls.mName, St::Lua_ReadProxyObject,
                      -ptrdiff_t(exposedMethod.mArguments.size()) - 1 + isLuaFunc);

      std::string funcArgs =
          GenerateReadExposedMethodArgsAndGetArgsString(exposedMethod, switchBlock, isLuaFunc);

      if (isLuaFunc) {
        if (funcArgs.empty()) {
          funcArgs = "lsInner";
        } else {
          funcArgs = std::format("lsInner, {}", funcArgs);
        }
      }

      if (exposedMethod.mReturnType.mName != "void") {
        if (exposedMethod.mReturnType.mType == PassByType::Reference) {
          switchBlock.Add("auto& result = instance->{}({});", exposedMethod.mName, funcArgs);
        } else {
          switchBlock.Add("auto result = instance->{}({});", exposedMethod.mName, funcArgs);
        }
        std::string accessor = ".";
        if (exposedMethod.mReturnType.mType == PassByType::Pointer) {
          accessor = "->";
        }
        auto returnedClass = mProject.GetClass(exposedMethod.mReturnType.mName);
        if (returnedClass && !returnedClass->mEnum) {
          if (exposedMethod.mReturnType.mType == PassByType::Value) {
            switchBlock.Add("result{}{}(lsInner);", accessor, St::Lua_PushMirrorObject);
          } else {
            switchBlock.Add("result{}PushToLua(lsInner);", accessor);
          }
        } else {
          std::string mirrorArg = "false";
          if (exposedMethod.mReturnType.mType == PassByType::Value) {
            mirrorArg = "true";
          }
          switchBlock.Add("{}::{}(result, lsInner, {});", St::LuaHelper, St::LuaHelper_Push,
                          mirrorArg);
        }
        switchBlock.Add("return 1;");
      } else {
        switchBlock.Add("instance->{}({});", exposedMethod.mName, funcArgs);
        switchBlock.Add("return 0;");
      }
      switchBlock.Indent(-1);
      switchBlock.Add("}});");
    });
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexForField(Class &cls, ClassField &field,
                                                     CodeBlock &switchBlock) {
  if (field.mField->mType.mName == St::Variant) {
    GenerateIndexForVariantField(cls, field, switchBlock);
  } else if (field.mField->mType.mName == St::Lua_CustomData) {
    GenerateIndexForRegistryData(field, switchBlock);
  } else {
    switchBlock.Add("{}::{}(instance->{}, luaState, false);", St::LuaHelper, St::LuaHelper_Push,
                    field.mName);
  }
}

std::string LuaIndexMetaMethodPlugin::GenerateReadExposedMethodArgsAndGetArgsString(
    ClassMethod &exposedMethod, CodeBlock &switchBlock, bool isLuaFunc) {
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
      bool canBeMirror = true;
      bool canBeProxy = true;
      if (arg.mType.mType != PassByType::Value && arg.mType.mConstness == Constness::NotConst) {
        canBeMirror = false;
      }
      if (argClass->mEnum) {
        canBeProxy = false;
      }
      if (canBeProxy && canBeMirror) {
        switchBlock.Add("{} arg{}Mirror;", arg.mType.mName, i);
        switchBlock.Add("{} *arg{};", arg.mType.mName, i);
        switchBlock.Add("if (lua_getmetatable(lsInner, {})) {{", stackIdx);
        switchBlock.Indent(1);
        switchBlock.Add("lua_pop(lsInner, 1);");
        switchBlock.Add("arg{} = {}::{}(lsInner, {});", i, arg.mType.mName, St::Lua_ReadProxyObject,
                        stackIdx);
        switchBlock.Indent(-1);
        switchBlock.Add("}} else {{");
        switchBlock.Indent(1);
        switchBlock.Add("arg{}Mirror = {}::{}(lsInner, {});", i, arg.mType.mName,
                        St::Lua_ReadMirrorObject, stackIdx);
        switchBlock.Add("arg{0} = &arg{0}Mirror;", i);
        switchBlock.Indent(-1);
        switchBlock.Add("}}");
      } else if (canBeProxy) {
        switchBlock.Add("auto arg{} = {}::{}(lsInner, {});", i, arg.mType.mName,
                        St::Lua_ReadProxyObject, stackIdx);
      } else if (canBeMirror) {
        switchBlock.Add("auto arg{} = {}::{}(lsInner, {});", i, arg.mType.mName,
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
      switchBlock.Add("{}::{}(arg{}, lsInner, {});", St::LuaHelper, St::LuaHelper_Read, i,
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
    switchBlock.Add("{}::{}(instance->{}(), luaState, false);", St::LuaHelper, St::LuaHelper_Push,
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

} // namespace holgen
