// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructType.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestVariantStructType::TestVariantStructType(TestVariantStructType::UnderlyingType value) :
    mValue(TestVariantStructType::Entry(value)) {
}

TestVariantStructType::TestVariantStructType(Entry value) :
    mValue(value) {
}

TestVariantStructType::Entry TestVariantStructType::GetValue() const {
  return Entry(mValue);
}

TestVariantStructType TestVariantStructType::FromString(std::string_view str) {
  if (str == "Human") {
    return TestVariantStructType(TestVariantStructType::Human);
  } else if (str == "Cat") {
    return TestVariantStructType(TestVariantStructType::Cat);
  } else {
    HOLGEN_WARN("{} is not a valid TestVariantStructType, returning invalid", str);
    return TestVariantStructType{TestVariantStructType::Invalid};
  }
}

const char *TestVariantStructType::ToString() const {
  switch (mValue) {
    case TestVariantStructType::Human:
      return "Human";
    case TestVariantStructType::Cat:
      return "Cat";
    default:
      return "INVALID";
  }
}

TestVariantStructType &TestVariantStructType::operator=(UnderlyingType rhs) {
  mValue = TestVariantStructType::Entry(rhs);
  return *this;
}

bool TestVariantStructType::operator==(UnderlyingType rhs) const {
  return mValue == TestVariantStructType::Entry(rhs);
}

bool TestVariantStructType::operator==(const TestVariantStructType &rhs) const {
  return mValue == rhs.mValue;
}

bool TestVariantStructType::operator!=(UnderlyingType rhs) const {
  return mValue != TestVariantStructType::Entry(rhs);
}

bool TestVariantStructType::operator!=(const TestVariantStructType &rhs) const {
  return mValue != rhs.mValue;
}

bool TestVariantStructType::operator<(UnderlyingType rhs) const {
  return mValue < TestVariantStructType::Entry(rhs);
}

bool TestVariantStructType::operator<(const TestVariantStructType &rhs) const {
  return mValue < rhs.mValue;
}

bool TestVariantStructType::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsString()) {
    *this = TestVariantStructType::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestVariantStructType(json.GetInt64());
  } else {
    *this = TestVariantStructType{};
    HOLGEN_WARN("Could not json-parse TestVariantStructType enum: invalid json type");
    return false;
  }
  return true;
}

void TestVariantStructType::PushToLua(lua_State *luaState) const {
  LuaHelper::Push(TestVariantStructType::UnderlyingType(mValue), luaState, true);
}

void TestVariantStructType::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push(TestVariantStructType::UnderlyingType(mValue), luaState, true);
}

TestVariantStructType TestVariantStructType::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestVariantStructType(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing TestVariantStructType: {}", lua_typename(luaState, typ));
    return TestVariantStructType{};
  }
}

void TestVariantStructType::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Human");
  lua_pushnumber(luaState, TestVariantStructType::Human);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Cat");
  lua_pushnumber(luaState, TestVariantStructType::Cat);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 2);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Human");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "Cat");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 2);
  lua_setglobal(luaState, "TestVariantStructType");
}
}
