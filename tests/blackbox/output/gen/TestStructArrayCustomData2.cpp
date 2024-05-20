// This file is generated by holgen. Do not modify manually.
#include "TestStructArrayCustomData2.h"

#include <cmath>
#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
bool TestStructArrayCustomData2::operator==(const TestStructArrayCustomData2& rhs) const {
  return
      mF1 == rhs.mF1 &&
      std::fabs(mF2 - rhs.mF2) < 0.00001 &&
      mF3 == rhs.mF3;
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

bool TestStructArrayCustomData2::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestStructArrayCustomData2");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
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
  return true;
}

void TestStructArrayCustomData2::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructArrayCustomData2Meta");
  lua_setmetatable(luaState, -2);
}

void TestStructArrayCustomData2::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructArrayCustomData2* TestStructArrayCustomData2::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructArrayCustomData2*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestStructArrayCustomData2::IndexMetaMethod(lua_State* luaState) {
  auto instance = TestStructArrayCustomData2::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("f1", key)) {
    LuaHelper::Push(instance->mF1, luaState);
  } else if (0 == strcmp("f2", key)) {
    LuaHelper::Push(instance->mF2, luaState);
  } else if (0 == strcmp("f3", key)) {
    LuaHelper::Push(instance->mF3, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData2.{}", key);
    return 0;
  }
  return 1;
}

int TestStructArrayCustomData2::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = TestStructArrayCustomData2::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("f1", key)) {
    LuaHelper::Read(instance->mF1, luaState, -1);
  } else if (0 == strcmp("f2", key)) {
    LuaHelper::Read(instance->mF2, luaState, -1);
  } else if (0 == strcmp("f3", key)) {
    LuaHelper::Read(instance->mF3, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData2.{}", key);
  }
  return 0;
}

void TestStructArrayCustomData2::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructArrayCustomData2::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructArrayCustomData2::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructArrayCustomData2Meta");
}
}