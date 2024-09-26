// This file is generated by holgen. Do not modify manually.
#include "TestContainerInnerStructNoId.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerInnerStructNoId::operator==(const TestContainerInnerStructNoId &rhs) const {
  return !(
      mField != rhs.mField ||
      mName != rhs.mName
  );
}

uint32_t TestContainerInnerStructNoId::GetField() const {
  return mField;
}

const std::string &TestContainerInnerStructNoId::GetName() const {
  return mName;
}

std::string &TestContainerInnerStructNoId::GetName() {
  return mName;
}

void TestContainerInnerStructNoId::SetField(uint32_t val) {
  mField = val;
}

void TestContainerInnerStructNoId::SetName(const std::string &val) {
  mName = val;
}

bool TestContainerInnerStructNoId::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("field", name)) {
        auto res = JsonHelper::Parse(mField, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructNoId.field field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructNoId.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestContainerInnerStructNoId: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerInnerStructNoId!");
      auto res = JsonHelper::Parse(mField, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructNoId.field field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerInnerStructNoId!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructNoId.name field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestContainerInnerStructNoId!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestContainerInnerStructNoId.");
    return false;
  }
  return true;
}

void TestContainerInnerStructNoId::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerInnerStructNoIdMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerInnerStructNoId::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "field");
  LuaHelper::Push(mField, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
}

void TestContainerInnerStructNoId::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerInnerStructNoId *TestContainerInnerStructNoId::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerInnerStructNoId *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerInnerStructNoId TestContainerInnerStructNoId::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerInnerStructNoId{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("field", key)) {
      LuaHelper::Read(result.mField, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructNoId.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerInnerStructNoId::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerInnerStructNoId::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("field", key)) {
    auto res = LuaHelper::Read(instance->mField, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerInnerStructNoId.field from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerInnerStructNoId.name from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructNoId.{}", key);
  }
  return 0;
}

void TestContainerInnerStructNoId::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerInnerStructNoId::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerInnerStructNoId::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerInnerStructNoIdMeta");
}

int TestContainerInnerStructNoId::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("field", key)) {
    auto instance = TestContainerInnerStructNoId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerInnerStructNoId.field with an invalid lua proxy object!");
    LuaHelper::Push(instance->mField, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = TestContainerInnerStructNoId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerInnerStructNoId.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructNoId.{}", key);
    return 0;
  }
  return 1;
}
}
