// This file is generated by holgen. Do not modify manually.
#include "TestStructNonCopyable.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
bool TestStructNonCopyable::operator==(const TestStructNonCopyable &rhs) const {
  return
      mBigVector == rhs.mBigVector;
}

const std::vector<int> &TestStructNonCopyable::GetBigVector() const {
  return mBigVector;
}

std::vector<int> &TestStructNonCopyable::GetBigVector() {
  return mBigVector;
}

void TestStructNonCopyable::SetBigVector(const std::vector<int> &val) {
  mBigVector = val;
}

bool TestStructNonCopyable::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("bigVector", name)) {
        auto res = JsonHelper::Parse(mBigVector, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructNonCopyable.bigVector field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructNonCopyable: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mBigVector, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructNonCopyable.bigVector field");
  }
  return true;
}

void TestStructNonCopyable::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructNonCopyableMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructNonCopyable::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructNonCopyable *TestStructNonCopyable::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructNonCopyable *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestStructNonCopyable::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructNonCopyable::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("bigVector", key)) {
    LuaHelper::Push(instance->mBigVector, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructNonCopyable.{}", key);
    return 0;
  }
  return 1;
}

int TestStructNonCopyable::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructNonCopyable::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("bigVector", key)) {
    LuaHelper::Read(instance->mBigVector, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructNonCopyable.{}", key);
  }
  return 0;
}

void TestStructNonCopyable::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructNonCopyable::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructNonCopyable::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructNonCopyableMeta");
}
}