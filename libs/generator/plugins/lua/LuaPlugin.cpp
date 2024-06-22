#include "LuaPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/StringSwitcher.h"
#include "core/Exception.h"

namespace holgen {
namespace {
std::string LuaTableField_Pointer = "p";
std::string LuaTableField_Index = "i";
std::string LuaTableField_Type = "t";
} // namespace

void LuaPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void LuaPlugin::GenerateIndexMetaMethodForFields(Class &cls, StringSwitcher &switcher) {
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
          switchBlock.Add("{}::{}({}::{}(instance->{}), luaState);", St::LuaHelper,
                          St::LuaHelper_Push, field.mField->mType.mTemplateParameters[0].mName,
                          St::ManagedObject_Getter, field.mName);
        });
      }
    }
  }
}

void LuaPlugin::GenerateIndexMetaMethodForExposedMethods(Class &cls, StringSwitcher &switcher) {
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
          switchBlock.Add("{}::{}(result, lsInner);", St::LuaHelper, St::LuaHelper_Push);
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

std::string LuaPlugin::GenerateReadExposedMethodArgsAndGetArgsString(ClassMethod &exposedMethod,
                                                                     CodeBlock &switchBlock,
                                                                     bool isLuaFunc) {
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

void LuaPlugin::GenerateIndexMetaMethod(Class &cls) {
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

void LuaPlugin::GenerateIndexForVariantField(Class &cls, ClassField &field,
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
    switchBlock.Add("{}::{}(instance->{}(), luaState);", St::LuaHelper, St::LuaHelper_Push,
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

void LuaPlugin::GenerateIndexForRegistryData(ClassField &field, CodeBlock &switchBlock) {
  switchBlock.Add("lua_rawgeti(luaState, LUA_REGISTRYINDEX, instance->{});", field.mName);
}

void LuaPlugin::GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock) {
  if (field.mField->mType.mName == St::Variant) {
    GenerateIndexForVariantField(cls, field, switchBlock);
  } else if (field.mField->mType.mName == St::Lua_RegistryData) {
    GenerateIndexForRegistryData(field, switchBlock);
  } else {
    switchBlock.Add("{}::{}(instance->{}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                    field.mName);
  }
}

void LuaPlugin::GenerateNewIndexMetaMethod(Class &cls) {
  auto method = ClassMethod{"NewIndexMetaMethod", Type{"int"}, Visibility::Private,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(R"R(HOLGEN_WARN("Unexpected lua field: {}.{{}}", key);)R",
                             cls.mStruct->mName);
  StringSwitcher switcher("key", std::move(stringSwitcherElseCase));
  const std::set<std::string> NoNewIndexTypes = {St::UserData, St::Variant, St::Lua_RegistryData};
  for (auto &field: cls.mFields) {
    // TODO: parse variant
    if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua) ||
        NoNewIndexTypes.contains(field.mField->mType.mName))
      continue;
    if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Const))
      continue;
    // TODO: Make this work with pointers
    // TODO: Make this work with nested structs
    // TODO: This appends to containers, so a=[1] a=[2] results in a=[1,2].
    switcher.AddCase(Naming().FieldNameInLua(*field.mField), [&](CodeBlock &switchBlock) {
      switchBlock.Add("{}::{}(instance->{}, luaState, -1);", St::LuaHelper, St::LuaHelper_Read,
                      field.mName);
    });
  }
  if (!switcher.IsEmpty()) {
    method.mBody.Add("auto instance = {}::{}(luaState, -3);", cls.mName, St::Lua_ReadProxyObject);
    method.mBody.Add("const char *key = lua_tostring(luaState, -2);");
    method.mBody.Add(std::move(switcher.Generate()));
  }
  method.mBody.Line() << "return 0;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateReadProxyObjectFromLua(Class &cls) {
  auto method = ClassMethod{St::Lua_ReadProxyObject, Type{cls.mName, PassByType::Pointer},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mComments.emplace_back("This only works with negative indices");
  method.mComments.emplace_back(
      "Reads proxy object (a table with a metatable and an embedded pointer or an index)");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("idx", Type{"int32_t"});
  THROW_IF(cls.mEnum, "Proxy objects only work with structs!")
  GenerateReadProxyStructFromLuaBody(cls, method);

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateReadMirrorObjectFromLua(Class &cls) {
  auto method = ClassMethod{St::Lua_ReadMirrorObject, Type{cls.mName, PassByType::Value},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mComments.emplace_back("This only works with negative indices");
  method.mComments.emplace_back(
      "Reads a mirror object (a table with entries that mirror the c++ data structure)");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("idx", Type{"int32_t"});
  if (cls.mEnum) {
    GenerateReadEnumFromLuaBody(cls, method);
  } else {
    GenerateReadMirrorStructFromLuaBody(cls, method);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateReadProxyStructFromLuaBody(Class &cls, ClassMethod &method) {
  if (!ShouldEmbedPointer(cls)) {
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
    method.mBody.Add("lua_gettable(luaState, idx - 1);");
    auto idField = cls.GetIdField();
    std::string tempType = "uint64_t";
    if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
      tempType = "int64_t";
    }
    method.mBody.Add("{} id = reinterpret_cast<{}>(lua_touserdata(luaState, -1));",
                     idField->mType.ToString(true), tempType);
    method.mBody.Add("auto ptr = {}::{}(id);", cls.mName, St::ManagedObject_Getter);
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return ptr;");
  } else {
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
    method.mBody.Add("lua_gettable(luaState, idx - 1);");
    method.mBody.Add("auto ptr = ({} *) lua_touserdata(luaState, -1);", cls.mName);
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return ptr;");
  }
}

void LuaPlugin::GenerateReadMirrorStructFromLuaBody(Class &cls, ClassMethod &method) {
  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(R"R(HOLGEN_WARN("Unexpected lua field: {}.{{}}", key);)R",
                             cls.mStruct->mName);
  // TODO: uncomment this to remove unnecessary lua_pushvalue for registry data
  // stringSwitcherElseCase.Add("lua_pop(luaState, 1);");
  StringSwitcher switcher("key", std::move(stringSwitcherElseCase));
  for (auto &field: cls.mFields) {
    // TODO: handle variant type (should call setter instead of direct assignment)
    if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua) ||
        field.mField->mType.mName == St::UserData || field.mField->mType.mName == St::Variant)
      continue;
    // TODO: handle refs?
    auto fieldClass = mProject.GetClass(field.mType.mName);
    if (fieldClass && fieldClass->mStruct) {
      switcher.AddCase(
          Naming().FieldNameInLua(*field.mField), [&, fieldClass](CodeBlock &switchBlock) {
            bool canBeProxy = !fieldClass->mStruct->GetMatchingAttribute(
                Annotations::Struct, Annotations::Struct_NonCopyable);
            if (canBeProxy) {
              switchBlock.Add("if (lua_getmetatable(luaState, -1)) {{");
              switchBlock.Indent(1);
              switchBlock.Add("lua_pop(luaState, 1);");
              std::string convertOp;
              if (field.mType.mType != PassByType::Pointer) {
                convertOp = "*";
              }
              switchBlock.Add("result.{} = {}{}::{}(luaState, -1);", field.mName, convertOp,
                              fieldClass->mName, St::Lua_ReadProxyObject);
              switchBlock.Indent(-1);
              switchBlock.Add("}} else {{");
              switchBlock.Indent(1);
            }
            if (field.mType.mType == PassByType::Pointer) {
              switchBlock.Add("HOLGEN_FAIL(\"Cannot write a mirror object to a pointer!\");");
            } else {
              switchBlock.Add("result.{} = {}::{}(luaState, -1);", field.mName, fieldClass->mName,
                              St::Lua_ReadMirrorObject);
            }
            if (canBeProxy) {
              switchBlock.Indent(-1);
              switchBlock.Add("}}");
            }
            // TODO: uncomment this to remove unnecessary lua_pushvalue for registry data
            // switchBlock.Add("lua_pop(luaState, 1);");
          });
    } else if (field.mField && field.mField->mType.mName == St::Lua_RegistryData) {
      switcher.AddCase(Naming().FieldNameInLua(*field.mField), [&](CodeBlock &switchBlock) {
        // TODO: remove this in a separate commit
        switchBlock.Add("lua_pushvalue(luaState, -1);", field.mName);
        switchBlock.Add("result.{} = luaL_ref(luaState, LUA_REGISTRYINDEX);", field.mName);
      });
    } else {
      switcher.AddCase(Naming().FieldNameInLua(*field.mField), [&](CodeBlock &switchBlock) {
        switchBlock.Add("{}::{}(result.{}, luaState, -1);", St::LuaHelper, St::LuaHelper_Read,
                        field.mName);
        // TODO: uncomment this to remove unnecessary lua_pushvalue for registry data
        // switchBlock.Add("lua_pop(luaState, 1);");
      });
    }
  }
  if (switcher.IsEmpty()) {
    method.mBody.Add("return {}{{}};", cls.mName);
    return;
  }
  method.mBody.Add("auto result = {}{{}};", cls.mName);
  method.mBody.Add("lua_pushvalue(luaState, idx);", cls.mName);
  method.mBody.Add("lua_pushnil(luaState);");
  method.mBody.Add("while (lua_next(luaState, -2)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto key = lua_tostring(luaState, -2);");

  method.mBody.Add(std::move(switcher.Generate()));

  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return result;", cls.mName);
}

void LuaPlugin::GenerateReadEnumFromLuaBody(Class &cls, ClassMethod &method) {
  method.mReturnType.mType = PassByType::Value;
  method.mBody.Add("auto typ = lua_type(luaState, idx);");
  method.mBody.Add("if (typ == LUA_TSTRING) {{");
  method.mBody.Indent(1);
  method.mBody.Add("return FromString(lua_tostring(luaState, idx));");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else if (typ == LUA_TNUMBER) {{");
  method.mBody.Indent(1);
  method.mBody.Add("return {}(lua_tonumber(luaState, idx));", cls.mName);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("return {}{{}};", cls.mName);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void LuaPlugin::GeneratePushToLua(Class &cls) {
  auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("lua_newtable(luaState);");
  if (!ShouldEmbedPointer(cls)) {
    auto idField = cls.GetIdField();
    std::string tempType = "uint64_t";
    if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
      tempType = "int64_t";
    }
    method.mBody.Add("{} id = {};", tempType, idField->mName);
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
    method.mBody.Add("lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));");
  } else {
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
    method.mBody.Add("lua_pushlightuserdata(luaState, (void *) this);");
  }
  method.mBody.Add("lua_settable(luaState, -3);");
  /*
  // TODO: this should read from GeneratorSettings
  // can also just get the metatable that's already available?
  method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Type);
  method.mBody.Add("lua_pushstring(luaState, \"{}\");", cls.mName);
  method.mBody.Add("lua_settable(luaState, -3);");
  */
  // Do this last so that metamethods don't get called during object construction
  method.mBody.Add("lua_getglobal(luaState, \"{}\");", Naming().LuaMetatableName(cls));
  method.mBody.Add("lua_setmetatable(luaState, -2);");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GeneratePushMirrorStructToLua(Class &cls) {
  auto method =
      ClassMethod{St::Lua_PushMirrorObject, Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("lua_newtable(luaState);");
  for (auto &field: cls.mFields) {
    if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua) ||
        field.mField->mType.mName == St::UserData || field.mField->mType.mName == St::Variant) {
      continue;
    }
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", field.mField->mName);
    auto fieldClass = mProject.GetClass(field.mType.mName);
    if (fieldClass && !fieldClass->mEnum) {
      method.mBody.Add("{}.{}(luaState);", field.mName, St::Lua_PushMirrorObject);
    } else if (field.mField && field.mField->mType.mName == St::Lua_RegistryData) {
      method.mBody.Add("lua_rawgeti(luaState, LUA_REGISTRYINDEX, {});", field.mName);
    } else {
      method.mBody.Add("{}::{}({}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                       Naming().FieldNameInCpp(*field.mField, false));
    }
    method.mBody.Add("lua_settable(luaState, -3);");
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::ProcessStruct(Class &cls) {
  if (cls.mStruct->GetAnnotation(Annotations::NoLua))
    return;
  // TODO: just include in header
  // cls.mHeaderIncludes.AddLibHeader("lua.hpp");
  cls.mHeaderIncludes.AddForwardDeclaration({"", "struct", "lua_State"});
  cls.mSourceIncludes.AddLibHeader("lua.hpp");
  cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
  GeneratePushToLua(cls);
  GeneratePushMirrorStructToLua(cls);
  GeneratePushGlobalToLua(cls);
  GenerateReadProxyObjectFromLua(cls);
  GenerateReadMirrorObjectFromLua(cls);
  GenerateIndexMetaMethod(cls);
  GenerateNewIndexMetaMethod(cls);
  GenerateCreateLuaMetatable(cls);
}

void LuaPlugin::GeneratePushEnumToLua(Class &cls) {
  auto method = ClassMethod{"PushEnumToLua", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");

  for (auto &entry: cls.mEnum->mEntries) {
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", entry.mName);
    method.mBody.Add("lua_pushnumber(luaState, {});", entry.mValue);
    method.mBody.Add("lua_settable(luaState, -3);");
  }
  // TODO: use const for invalid entry name
  method.mBody.Add("lua_pushstring(luaState, \"Invalid\");");
  method.mBody.Add("lua_pushnumber(luaState, {});", cls.mEnum->mInvalidValue);
  method.mBody.Add("lua_settable(luaState, -3);");

  for (auto &entry: cls.mEnum->mEntries) {
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", entry.mName);
    method.mBody.Add("lua_rawseti(luaState, -2, {});", entry.mValue);
  }

  method.mBody.Add("lua_pushstring(luaState, \"Invalid\");");
  method.mBody.Add("lua_rawseti(luaState, -2, {});", cls.mEnum->mInvalidValue);

  method.mBody.Add("lua_setglobal(luaState, \"{}\");", cls.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateCreateLuaMetatable(Class &cls) {
  auto method = ClassMethod{"CreateLuaMetatable", Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");

  method.mBody.Add("lua_pushstring(luaState, \"__index\");");
  method.mBody.Add("lua_pushcfunction(luaState, {}::IndexMetaMethod);", cls.mName);
  method.mBody.Add("lua_settable(luaState, -3);");

  method.mBody.Add("lua_pushstring(luaState, \"__newindex\");");
  method.mBody.Add("lua_pushcfunction(luaState, {}::NewIndexMetaMethod);", cls.mName);
  method.mBody.Add("lua_settable(luaState, -3);");
  method.mBody.Add("lua_setglobal(luaState, \"{}\");", Naming().LuaMetatableName(cls));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::ProcessEnum(Class &cls) {
  if (cls.mEnum->GetAnnotation(Annotations::NoLua))
    return;
  cls.mHeaderIncludes.AddForwardDeclaration({"", "struct", "lua_State"});
  cls.mSourceIncludes.AddLibHeader("lua.hpp");
  cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
  auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("{}::{}(mValue, luaState);", St::LuaHelper, St::LuaHelper_Push);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
  GenerateReadMirrorObjectFromLua(cls);
  GeneratePushEnumToLua(cls);
}

void LuaPlugin::GeneratePushGlobalToLua(Class &cls) {
  auto method = ClassMethod{"PushGlobalToLua", Type{"void"}};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("name", Type{"char", PassByType::Pointer, Constness::Const});
  method.mBody.Add("PushToLua(luaState);");
  method.mBody.Add("lua_setglobal(luaState, name);");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

bool LuaPlugin::ShouldEmbedPointer(Class &cls) {
  auto managed = cls.mStruct->GetAnnotation(Annotations::Managed);
  if (!managed)
    return true;
  auto manager = mProject.GetClass(managed->GetAttribute(Annotations::Managed_By)->mValue.mName);
  auto field = manager->GetFieldFromDefinitionName(
      managed->GetAttribute(Annotations::Managed_Field)->mValue.mName);
  if (TypeInfo::Get().CppStableContainers.contains(field->mType.mName))
    return true;
  return false;
}
} // namespace holgen
