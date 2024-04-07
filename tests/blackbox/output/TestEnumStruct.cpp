// This file is generated by holgen. Do not modify manually.
#include "TestEnumStruct.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const TestEnum& TestEnumStruct::GetEnumField() const {
  return mEnumField;
}

TestEnum& TestEnumStruct::GetEnumField() {
  return mEnumField;
}

void TestEnumStruct::SetEnumField(const TestEnum& val) {
  mEnumField = val;
}

bool TestEnumStruct::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestEnumStruct");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("enumField", name)) {
      auto res = JsonHelper::Parse(mEnumField, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestEnumStruct.enumField field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestEnumStruct: {}", name);
    }
  }
  return true;
}

void TestEnumStruct::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestEnumStructMeta");
  lua_setmetatable(luaState, -2);
}

void TestEnumStruct::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestEnumStruct* TestEnumStruct::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestEnumStruct*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestEnumStruct::IndexMetaMethod(lua_State* luaState) {
  auto instance = TestEnumStruct::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("enumField", key)) {
    LuaHelper::Push(instance->mEnumField, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEnumStruct.{}", key);
    return 0;
  }
  return 1;
}

int TestEnumStruct::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = TestEnumStruct::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("enumField", key)) {
    LuaHelper::Read(instance->mEnumField, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEnumStruct.{}", key);
  }
  return 0;
}

void TestEnumStruct::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestEnumStruct::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestEnumStruct::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestEnumStructMeta");
}
}
