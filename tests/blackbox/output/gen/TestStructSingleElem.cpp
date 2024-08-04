// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElem.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructSingleElem::operator==(const TestStructSingleElem &rhs) const {
  return !(
      mName != rhs.mName
  );
}

const std::string &TestStructSingleElem::GetName() const {
  return mName;
}

std::string &TestStructSingleElem::GetName() {
  return mName;
}

void TestStructSingleElem::SetName(const std::string &val) {
  mName = val;
}

bool TestStructSingleElem::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElem.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructSingleElem: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mName, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElem.name field");
  }
  return true;
}

void TestStructSingleElem::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructSingleElem::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructSingleElem::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructSingleElem *TestStructSingleElem::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElem *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructSingleElem TestStructSingleElem::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructSingleElem{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElem.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructSingleElem::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructSingleElem::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElem.{}", key);
  }
  return 0;
}

void TestStructSingleElem::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructSingleElem::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructSingleElem::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructSingleElemMeta");
}

int TestStructSingleElem::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructSingleElem::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElem.{}", key);
    return 0;
  }
  return 1;
}
}
