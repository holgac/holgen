// This file is generated by holgen. Do not modify manually.
#include "TestStructArrayCustomData2.h"

#include <cmath>
#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructArrayCustomData2::operator==(const TestStructArrayCustomData2 &rhs) const {
  return !(
      mF1 != rhs.mF1 ||
      std::fabs(mF2 - rhs.mF2) >= 0.00001 ||
      mF3 != rhs.mF3
  );
}

uint64_t TestStructArrayCustomData2::GetF1() const {
  return mF1;
}

float TestStructArrayCustomData2::GetF2() const {
  return mF2;
}

uint32_t TestStructArrayCustomData2::GetF3() const {
  return mF3;
}

void TestStructArrayCustomData2::SetF1(uint64_t val) {
  mF1 = val;
}

void TestStructArrayCustomData2::SetF2(float val) {
  mF2 = val;
}

void TestStructArrayCustomData2::SetF3(uint32_t val) {
  mF3 = val;
}

bool TestStructArrayCustomData2::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("f1", name)) {
        auto res = JsonHelper::Parse(mF1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f1 field");
      } else if (0 == strcmp("f2", name)) {
        auto res = JsonHelper::Parse(mF2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f2 field");
      } else if (0 == strcmp("f3", name)) {
        auto res = JsonHelper::Parse(mF3, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f3 field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructArrayCustomData2: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData2!");
      auto res = JsonHelper::Parse(mF1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData2!");
      auto res = JsonHelper::Parse(mF2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f2 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData2!");
      auto res = JsonHelper::Parse(mF3, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData2.f3 field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructArrayCustomData2!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructArrayCustomData2.");
    return false;
  }
  return true;
}

void TestStructArrayCustomData2::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructArrayCustomData2Meta");
  lua_setmetatable(luaState, -2);
}

void TestStructArrayCustomData2::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "f1");
  LuaHelper::Push(mF1, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "f2");
  LuaHelper::Push(mF2, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "f3");
  LuaHelper::Push(mF3, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructArrayCustomData2::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructArrayCustomData2 *TestStructArrayCustomData2::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructArrayCustomData2 *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructArrayCustomData2 TestStructArrayCustomData2::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructArrayCustomData2{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("f1", key)) {
      LuaHelper::Read(result.mF1, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("f2", key)) {
      LuaHelper::Read(result.mF2, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("f3", key)) {
      LuaHelper::Read(result.mF3, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData2.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructArrayCustomData2::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructArrayCustomData2::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("f1", key)) {
    auto res = LuaHelper::Read(instance->mF1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData2.f1 from lua failed!");
  } else if (0 == strcmp("f2", key)) {
    auto res = LuaHelper::Read(instance->mF2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData2.f2 from lua failed!");
  } else if (0 == strcmp("f3", key)) {
    auto res = LuaHelper::Read(instance->mF3, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData2.f3 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData2.{}", key);
  }
  return 0;
}

void TestStructArrayCustomData2::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructArrayCustomData2::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructArrayCustomData2::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructArrayCustomData2Meta");
}

int TestStructArrayCustomData2::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("f1", key)) {
    auto instance = TestStructArrayCustomData2::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData2.f1 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mF1, luaState, false);
  } else if (0 == strcmp("f2", key)) {
    auto instance = TestStructArrayCustomData2::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData2.f2 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mF2, luaState, false);
  } else if (0 == strcmp("f3", key)) {
    auto instance = TestStructArrayCustomData2::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData2.f3 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mF3, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData2.{}", key);
    return 0;
  }
  return 1;
}
}
