// This file is generated by holgen. Do not modify manually.
#include "TestLuaNumber.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestLuaNumber::operator==(const TestLuaNumber &rhs) const {
  return !(
      mValue != rhs.mValue
  );
}

int32_t TestLuaNumber::GetValue() const {
  return mValue;
}

void TestLuaNumber::SetValue(int32_t val) {
  mValue = val;
}

bool TestLuaNumber::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("value", name)) {
        auto res = JsonHelper::Parse(mValue, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaNumber.value field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestLuaNumber: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaNumber!");
      auto res = JsonHelper::Parse(mValue, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaNumber.value field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestLuaNumber!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestLuaNumber.");
    return false;
  }
  return true;
}

void TestLuaNumber::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaNumberMeta");
  lua_setmetatable(luaState, -2);
}

void TestLuaNumber::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "value");
  LuaHelper::Push(mValue, luaState, true);
  lua_settable(luaState, -3);
}

void TestLuaNumber::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaNumber *TestLuaNumber::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaNumber *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaNumber TestLuaNumber::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestLuaNumber{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("value", key)) {
      LuaHelper::Read(result.mValue, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestLuaNumber.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestLuaNumber::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestLuaNumber::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("value", key)) {
    auto res = LuaHelper::Read(instance->mValue, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaNumber.value from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaNumber.{}", key);
  }
  return 0;
}

void TestLuaNumber::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaNumber::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaNumber::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaNumberMeta");
}

int TestLuaNumber::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("value", key)) {
    auto instance = TestLuaNumber::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaNumber.value with an invalid lua proxy object!");
    LuaHelper::Push(instance->mValue, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaNumber.{}", key);
    return 0;
  }
  return 1;
}
}
