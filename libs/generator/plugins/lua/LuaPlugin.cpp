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

void LuaPlugin::GenerateIndexMetaMethod(Class &cls) {
  auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(R"R(HOLGEN_WARN("Unexpected lua field: {}.{{}}", key);)R",
                             cls.mStruct->mName);
  stringSwitcherElseCase.Add("return 0;");
  StringSwitcher switcher("key", std::move(stringSwitcherElseCase));

  for (auto &field: cls.mFields) {
    // TODO: parse variant
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

  bool isLuaFuncTable = false;
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    isLuaFuncTable = true;
  }

  for (auto &exposedMethod: cls.mMethods) {
    if (!exposedMethod.mExposeToLua)
      continue;
    switcher.AddCase(exposedMethod.mName, [&](CodeBlock &switchBlock) {
      bool isLuaFunc = isLuaFuncTable;
      if (exposedMethod.mFunction && exposedMethod.mFunction->GetAnnotation(Annotations::LuaFunc)) {
        isLuaFunc = true;
      }

      // TODO: LuaHelper::Push should work with functions
      switchBlock.Add("lua_pushcfunction(luaState, [](lua_State *lsInner) {{");
      switchBlock.Indent(1);
      switchBlock.Add("auto instance = {}::ReadFromLua(lsInner, {});", cls.mName,
                      -ptrdiff_t(exposedMethod.mArguments.size()) - 1 + isLuaFunc);

      std::stringstream funcArgs;
      size_t i = 0;
      for (auto &arg: exposedMethod.mArguments) {
        if (arg.mType.mName == "lua_State") {
          THROW_IF(i != 0 || !isLuaFunc, "Unexpected lua_State argument!");
          continue;
        }
        if (i != 0)
          funcArgs << ", ";
        if (auto argClass = mProject.GetClass(arg.mType.mName)) {
          switchBlock.Add("auto arg{} = {}::ReadFromLua(lsInner, {});", i, arg.mType.mName,
                          ptrdiff_t(i) - ptrdiff_t(exposedMethod.mArguments.size()) + isLuaFunc);
          if (argClass->mStruct && arg.mType.mType != PassByType::Pointer)
            funcArgs << "*arg" << i;
          else
            funcArgs << "arg" << i;
        } else {
          auto sanitizedType = arg.mType;
          sanitizedType.mType = PassByType::Value;
          sanitizedType.mConstness = Constness::NotConst;
          switchBlock.Add("{}arg{};", sanitizedType.ToString(false), i);
          switchBlock.Add("{}::{}(arg{}, lsInner, {});", St::LuaHelper, St::LuaHelper_Read, i,
                          ptrdiff_t(i) - ptrdiff_t(exposedMethod.mArguments.size()));
          funcArgs << "arg" << i;
        }
        ++i;
      }

      std::string luaFuncPrefix;
      if (isLuaFunc) {
        if (i == 0) {
          luaFuncPrefix = "lsInner";
        } else {
          luaFuncPrefix = "lsInner, ";
        }
      }

      if (exposedMethod.mReturnType.mName != "void") {
        switchBlock.Add("auto result = instance->{}({}{});", exposedMethod.mName, luaFuncPrefix,
                        funcArgs.str());
        switchBlock.Add("{}::{}(result, lsInner);", St::LuaHelper, St::LuaHelper_Push);
        switchBlock.Add("return 1;");
      } else {
        switchBlock.Add("instance->{}({}{});", exposedMethod.mName, luaFuncPrefix, funcArgs.str());
        switchBlock.Add("return 0;");
      }
      switchBlock.Indent(-1);
      switchBlock.Add("}});");
    });
  }

  if (!switcher.IsEmpty()) {
    method.mBody.Add("auto instance = {}::ReadFromLua(luaState, -2);", cls.mName);
    method.mBody.Add("const char *key = lua_tostring(luaState, -1);");
    method.mBody.Add(std::move(switcher.Generate()));
    method.mBody.Line() << "return 1;";
  } else {
    method.mBody.Line() << "return 0;";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock) const {
  if (field.mField->mType.mName == St::Variant) {
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
  for (auto &field: cls.mFields) {
    // TODO: parse variant
    if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua) ||
        field.mField->mType.mName == St::UserData || field.mField->mType.mName == St::Variant)
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
    method.mBody.Add("auto instance = {}::ReadFromLua(luaState, -3);", cls.mName);
    method.mBody.Add("const char *key = lua_tostring(luaState, -2);");
    method.mBody.Add(std::move(switcher.Generate()));
  }
  method.mBody.Line() << "return 0;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateReadFromLua(Class &cls) {
  auto method = ClassMethod{"ReadFromLua", Type{cls.mName, PassByType::Pointer}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mComments.push_back("This only works with negative indices");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("idx", Type{"int32_t"});
  if (cls.mEnum) {
    GenerateReadEnumFromLuaBody(cls, method);
  } else {
    GenerateReadStructFromLuaBody(cls, method);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaPlugin::GenerateReadStructFromLuaBody(Class &cls, ClassMethod &method) {
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

void LuaPlugin::GenerateReadEnumFromLuaBody(Class &cls, ClassMethod &method) const {
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

void LuaPlugin::ProcessStruct(Class &cls) {
  if (cls.mStruct->GetAnnotation(Annotations::NoLua))
    return;
  cls.mHeaderIncludes.AddForwardDeclaration({"", "struct", "lua_State"});
  cls.mSourceIncludes.AddLibHeader("lua.hpp");
  cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
  GeneratePushToLua(cls);
  GeneratePushGlobalToLua(cls);
  GenerateReadFromLua(cls);
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
  GenerateReadFromLua(cls);
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
