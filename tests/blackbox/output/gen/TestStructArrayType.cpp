// This file is generated by holgen. Do not modify manually.
#include "TestStructArrayType.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestStructArrayType::TestStructArrayType(TestStructArrayType::UnderlyingType value) :
    mValue(value) {
}

TestStructArrayType::TestStructArrayType(Entry value) :
    mValue(UnderlyingType(value)) {
}

TestStructArrayType::Entry TestStructArrayType::GetValue() const {
  return Entry(mValue);
}

TestStructArrayType TestStructArrayType::FromString(std::string_view str) {
  if (str == "Type1") {
    return TestStructArrayType(TestStructArrayType::Type1);
  } else if (str == "Type2") {
    return TestStructArrayType(TestStructArrayType::Type2);
  } else {
    HOLGEN_WARN("{} is not a valid TestStructArrayType, returning invalid", str);
    return TestStructArrayType{};
  }
}

const char *TestStructArrayType::ToString() const {
  switch (mValue) {
    case TestStructArrayType::Type1:
      return "Type1";
    case TestStructArrayType::Type2:
      return "Type2";
    default:
      return "INVALID";
  }
}

TestStructArrayType &TestStructArrayType::operator=(UnderlyingType rhs) {
  mValue = rhs;
  return *this;
}

bool TestStructArrayType::operator==(UnderlyingType rhs) const {
  return mValue == rhs;
}

bool TestStructArrayType::operator==(const TestStructArrayType &rhs) const {
  return mValue == rhs.mValue;
}

bool TestStructArrayType::operator!=(UnderlyingType rhs) const {
  return mValue != rhs;
}

bool TestStructArrayType::operator!=(const TestStructArrayType &rhs) const {
  return mValue != rhs.mValue;
}

bool TestStructArrayType::operator<(UnderlyingType rhs) const {
  return mValue < rhs;
}

bool TestStructArrayType::operator<(const TestStructArrayType &rhs) const {
  return mValue < rhs.mValue;
}

bool TestStructArrayType::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsString()) {
    *this = TestStructArrayType::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestStructArrayType(json.GetInt64());
  } else {
    *this = TestStructArrayType{};
    HOLGEN_WARN("Could not json-parse TestStructArrayType enum: invalid json type");
    return false;
  }
  return true;
}

void TestStructArrayType::PushToLua(lua_State *luaState) const {
  LuaHelper::Push(mValue, luaState);
}

TestStructArrayType TestStructArrayType::ReadFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestStructArrayType(lua_tonumber(luaState, idx));
  } else {
    return TestStructArrayType{};
  }
}

void TestStructArrayType::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Type1");
  lua_pushnumber(luaState, 0);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Type2");
  lua_pushnumber(luaState, 1);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 2);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Type1");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "Type2");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 2);
  lua_setglobal(luaState, "TestStructArrayType");
}
}
