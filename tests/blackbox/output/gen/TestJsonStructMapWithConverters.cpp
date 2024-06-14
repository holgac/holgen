// This file is generated by holgen. Do not modify manually.
#include "TestJsonStructMapWithConverters.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestJsonStructMapWithConverters::operator==(const TestJsonStructMapWithConverters &rhs) const {
  return
      mTestMapConvertElem == rhs.mTestMapConvertElem &&
      mTestMapConvertKey == rhs.mTestMapConvertKey &&
      mTestMapConvertKeyElem == rhs.mTestMapConvertKeyElem;
}

const std::map<std::string, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertElem() const {
  return mTestMapConvertElem;
}

std::map<std::string, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertElem() {
  return mTestMapConvertElem;
}

const std::map<uint32_t, std::string> &TestJsonStructMapWithConverters::GetTestMapConvertKey() const {
  return mTestMapConvertKey;
}

std::map<uint32_t, std::string> &TestJsonStructMapWithConverters::GetTestMapConvertKey() {
  return mTestMapConvertKey;
}

const std::map<uint32_t, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertKeyElem() const {
  return mTestMapConvertKeyElem;
}

std::map<uint32_t, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertKeyElem() {
  return mTestMapConvertKeyElem;
}

void TestJsonStructMapWithConverters::SetTestMapConvertElem(const std::map<std::string, uint32_t> &val) {
  mTestMapConvertElem = val;
}

void TestJsonStructMapWithConverters::SetTestMapConvertKey(const std::map<uint32_t, std::string> &val) {
  mTestMapConvertKey = val;
}

void TestJsonStructMapWithConverters::SetTestMapConvertKeyElem(const std::map<uint32_t, uint32_t> &val) {
  mTestMapConvertKeyElem = val;
}

bool TestJsonStructMapWithConverters::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestJsonStructMapWithConverters");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("testMapConvertElem", name)) {
      auto res = JsonHelper::ParseConvertElem<std::string>(mTestMapConvertElem, data.value, converter, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertElem field");
    } else if (0 == strcmp("testMapConvertKey", name)) {
      auto res = JsonHelper::ParseConvertKey<std::string>(mTestMapConvertKey, data.value, converter, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKey field");
    } else if (0 == strcmp("testMapConvertKeyElem", name)) {
      auto res = JsonHelper::ParseConvertKeyElem<std::string, std::string>(mTestMapConvertKeyElem, data.value, converter, converter.testJsonConvertStringToU32, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKeyElem field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestJsonStructMapWithConverters: {}", name);
    }
  }
  return true;
}

void TestJsonStructMapWithConverters::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonStructMapWithConvertersMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonStructMapWithConverters::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonStructMapWithConverters *TestJsonStructMapWithConverters::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonStructMapWithConverters *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestJsonStructMapWithConverters::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructMapWithConverters::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("testMapConvertElem", key)) {
    LuaHelper::Push(instance->mTestMapConvertElem, luaState);
  } else if (0 == strcmp("testMapConvertKey", key)) {
    LuaHelper::Push(instance->mTestMapConvertKey, luaState);
  } else if (0 == strcmp("testMapConvertKeyElem", key)) {
    LuaHelper::Push(instance->mTestMapConvertKeyElem, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructMapWithConverters.{}", key);
    return 0;
  }
  return 1;
}

int TestJsonStructMapWithConverters::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructMapWithConverters::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("testMapConvertElem", key)) {
    LuaHelper::Read(instance->mTestMapConvertElem, luaState, -1);
  } else if (0 == strcmp("testMapConvertKey", key)) {
    LuaHelper::Read(instance->mTestMapConvertKey, luaState, -1);
  } else if (0 == strcmp("testMapConvertKeyElem", key)) {
    LuaHelper::Read(instance->mTestMapConvertKeyElem, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructMapWithConverters.{}", key);
  }
  return 0;
}

void TestJsonStructMapWithConverters::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonStructMapWithConverters::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonStructMapWithConverters::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonStructMapWithConvertersMeta");
}
}