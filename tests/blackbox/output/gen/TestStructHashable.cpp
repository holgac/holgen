// This file is generated by holgen. Do not modify manually.
#include "TestStructHashable.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructHashable::operator==(const TestStructHashable &rhs) const {
  return !(
      mField1 != rhs.mField1 ||
      mField2 != rhs.mField2
  );
}

uint32_t TestStructHashable::GetField1() const {
  return mField1;
}

uint32_t TestStructHashable::GetField2() const {
  return mField2;
}

void TestStructHashable::SetField1(uint32_t val) {
  mField1 = val;
}

void TestStructHashable::SetField2(uint32_t val) {
  mField2 = val;
}

bool TestStructHashable::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("field1", name)) {
        auto res = JsonHelper::Parse(mField1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashable.field1 field");
      } else if (0 == strcmp("field2", name)) {
        auto res = JsonHelper::Parse(mField2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashable.field2 field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructHashable: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructHashable!");
      auto res = JsonHelper::Parse(mField1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashable.field1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructHashable!");
      auto res = JsonHelper::Parse(mField2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashable.field2 field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructHashable!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructHashable.");
    return false;
  }
  return true;
}

void TestStructHashable::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructHashableMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructHashable::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "field1");
  LuaHelper::Push(mField1, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "field2");
  LuaHelper::Push(mField2, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructHashable::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructHashable *TestStructHashable::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructHashable", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructHashable *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructHashable TestStructHashable::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructHashable{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("field1", key)) {
      LuaHelper::Read(result.mField1, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("field2", key)) {
      LuaHelper::Read(result.mField2, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructHashable.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructHashable::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructHashable::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("field1", key)) {
    auto res = LuaHelper::Read(instance->mField1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructHashable.field1 from lua failed!");
  } else if (0 == strcmp("field2", key)) {
    auto res = LuaHelper::Read(instance->mField2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructHashable.field2 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructHashable.{}", key);
  }
  return 0;
}

void TestStructHashable::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructHashable::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructHashable::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructHashableMeta");
}

int TestStructHashable::HashCallerFromLua(lua_State *luaState) {
  auto instance = TestStructHashable::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructHashable.Hash method with an invalid lua proxy object!");
  auto result = instance->Hash();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestStructHashable::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("field1", key)) {
    auto instance = TestStructHashable::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructHashable.field1 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mField1, luaState, false);
  } else if (0 == strcmp("field2", key)) {
    auto instance = TestStructHashable::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructHashable.field2 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mField2, luaState, false);
  } else if (0 == strcmp("Hash", key)) {
    lua_pushcfunction(luaState, TestStructHashable::HashCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructHashable.{}", key);
    return 0;
  }
  return 1;
}
}
