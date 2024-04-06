// This file is generated by holgen. Do not modify manually.
#include "TestStruct.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStruct::GetTestFieldBool() const {
  return mTestFieldBool;
}
uint32_t TestStruct::GetTestFieldUnsigned() const {
  return mTestFieldUnsigned;
}
const std::string& TestStruct::GetTestFieldString() const {
  return mTestFieldString;
}
std::string& TestStruct::GetTestFieldString() {
  return mTestFieldString;
}
void TestStruct::SetTestFieldBool(bool val) {
  mTestFieldBool = val;
}
void TestStruct::SetTestFieldUnsigned(uint32_t val) {
  mTestFieldUnsigned = val;
}
void TestStruct::SetTestFieldString(const std::string& val) {
  mTestFieldString = val;
}
bool TestStruct::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestStruct");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("testFieldBool", name)) {
      auto res = JsonHelper::Parse(mTestFieldBool, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStruct.testFieldBool field");
    } else if (0 == strcmp("testFieldUnsigned", name)) {
      auto res = JsonHelper::Parse(mTestFieldUnsigned, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStruct.testFieldUnsigned field");
    } else if (0 == strcmp("testFieldString", name)) {
      auto res = JsonHelper::Parse(mTestFieldString, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStruct.testFieldString field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestStruct: {}", name);
    }
  }
  return true;
}
void TestStruct::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructMeta");
  lua_setmetatable(luaState, -2);
}
void TestStruct::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestStruct* TestStruct::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStruct*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestStruct::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStruct::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("testFieldBool", key)) {
      LuaHelper::Push(instance->mTestFieldBool, ls);
    } else if (0 == strcmp("testFieldUnsigned", key)) {
      LuaHelper::Push(instance->mTestFieldUnsigned, ls);
    } else if (0 == strcmp("testFieldString", key)) {
      LuaHelper::Push(instance->mTestFieldString, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void TestStruct::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStruct::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("testFieldBool", key)) {
      LuaHelper::Read(instance->mTestFieldBool, ls, -1);
    } else if (0 == strcmp("testFieldUnsigned", key)) {
      LuaHelper::Read(instance->mTestFieldUnsigned, ls, -1);
    } else if (0 == strcmp("testFieldString", key)) {
      LuaHelper::Read(instance->mTestFieldString, ls, -1);
    } else if (0 == strcmp("testFieldUserdata", key)) {
      LuaHelper::Read(instance->mTestFieldUserdata, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestStruct::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestStructMeta");
}
}
