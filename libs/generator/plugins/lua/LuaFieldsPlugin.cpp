#include "LuaFieldsPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/utils/StringSwitcher.h"
#include "core/Exception.h"

namespace holgen {
void LuaFieldsPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct) {
      ProcessStruct(cls, *cls.mStruct);
    }
  }
}

void LuaFieldsPlugin::AddRegistryDataField(Class &cls, const FieldDefinition &fieldDefinition) {
  auto field = ClassField{Naming().FieldNameInCpp(fieldDefinition.mName), Type{"int"}};
  field.mDefaultValue = "LUA_NOREF";
  field.mField = &fieldDefinition;

  Validate().NewField(cls, field);
  auto &fieldRef = cls.mFields.emplace_back(std::move(field));
  AddRegistryDataInitializer(cls, fieldRef);
  AddRegistryDataUninitializer(cls, fieldRef);
}

void LuaFieldsPlugin::AddRegistryDataUninitializer(Class &cls, ClassField &field) {
  if (!field.mField->GetMatchingAttribute(Annotations::LuaData, Annotations::LuaData_Shared)) {
    cls.mDestructor.mBody.Add("HOLGEN_WARN_IF({} != LUA_NOREF, \"{}.{} was not released!\");",
                              field.mName, cls.mName, field.mField->mName);
  }
  if (!field.mField->GetMatchingAttribute(Annotations::LuaData, Annotations::LuaData_NoInit)) {
    auto &method = GetLuaUninitializer(cls);
    method.mBody.Add("luaL_unref(luaState, LUA_REGISTRYINDEX, {});", field.mName);
    method.mBody.Add("{} = LUA_NOREF;", field.mName);
  }
}

void LuaFieldsPlugin::AddRegistryDataInitializer(Class &cls, ClassField &field) {
  if (!field.mField->GetMatchingAttribute(Annotations::LuaData, Annotations::LuaData_NoInit)) {
    auto &method = GetLuaInitializer(cls);
    method.mBody.Add("HOLGEN_WARN_IF({} != LUA_NOREF, \"{}.{} was already initialized!\");",
                     field.mName, cls.mName, field.mField->mName);
    method.mBody.Add("lua_newtable(luaState);");
    method.mBody.Add("{} = luaL_ref(luaState, LUA_REGISTRYINDEX);", field.mName);
  }
}

void LuaFieldsPlugin::ProcessRegistryDataField(Class &cls, const FieldDefinition &fieldDefinition) {
  // TODO: remove the line below after making LuaPlugin include lua.hpp in the header
  cls.mHeaderIncludes.AddLibHeader("lua.hpp");
  AddRegistryDataField(cls, fieldDefinition);
}

void LuaFieldsPlugin::ProcessStruct(Class &cls, const StructDefinition &structDefinition) {
  if (structDefinition.GetMatchingAttribute(Annotations::No, Annotations::No_Lua)) {
    return;
  }

  for (auto &mixin: structDefinition.mMixins) {
    ProcessStruct(cls, *mProject.mProject.GetStruct(mixin));
  }

  for (auto &field: structDefinition.mFields) {
    if (field.mType.mName == St::Lua_CustomData) {
      ProcessRegistryDataField(cls, field);
    }
  }
}

ClassMethod &LuaFieldsPlugin::GetLuaInitializer(Class &cls) {
  auto methodPtr = cls.GetMethod(St::Lua_Initialize, Constness::NotConst);
  if (methodPtr) {
    return *methodPtr;
  }
  auto method =
      ClassMethod{St::Lua_Initialize, Type{"void"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  Validate().NewMethod(cls, method);
  return cls.mMethods.emplace_back(std::move(method));
}

ClassMethod &LuaFieldsPlugin::GetLuaUninitializer(Class &cls) {
  auto methodPtr = cls.GetMethod(St::Lua_Uninitialize, Constness::NotConst);
  if (methodPtr) {
    return *methodPtr;
  }
  auto method =
      ClassMethod{St::Lua_Uninitialize, Type{"void"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  Validate().NewMethod(cls, method);
  return cls.mMethods.emplace_back(std::move(method));
}
} // namespace holgen
