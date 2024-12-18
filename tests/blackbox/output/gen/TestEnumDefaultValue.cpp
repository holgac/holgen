// This file is generated by holgen. Do not modify manually.
#include "TestEnumDefaultValue.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestEnumDefaultValue::TestEnumDefaultValue(TestEnumDefaultValue::UnderlyingType value) :
    mValue(TestEnumDefaultValue::Entry(value)) {
}

TestEnumDefaultValue::TestEnumDefaultValue(Entry value) :
    mValue(value) {
}

TestEnumDefaultValue::Entry TestEnumDefaultValue::GetValue() const {
  return Entry(mValue);
}

TestEnumDefaultValue TestEnumDefaultValue::FromString(std::string_view str) {
  if (str == "SomeEntry") {
    return TestEnumDefaultValue(TestEnumDefaultValue::SomeEntry);
  } else if (str == "DefaultEntry") {
    return TestEnumDefaultValue(TestEnumDefaultValue::DefaultEntry);
  } else if (str == "AnotherEntry") {
    return TestEnumDefaultValue(TestEnumDefaultValue::AnotherEntry);
  } else {
    HOLGEN_WARN("{} is not a valid TestEnumDefaultValue, returning invalid", str);
    return TestEnumDefaultValue{TestEnumDefaultValue::Invalid};
  }
}

const char *TestEnumDefaultValue::ToString() const {
  switch (mValue) {
    case TestEnumDefaultValue::SomeEntry:
      return "SomeEntry";
    case TestEnumDefaultValue::DefaultEntry:
      return "DefaultEntry";
    case TestEnumDefaultValue::AnotherEntry:
      return "AnotherEntry";
    default:
      return "INVALID";
  }
}

TestEnumDefaultValue &TestEnumDefaultValue::operator=(UnderlyingType rhs) {
  mValue = TestEnumDefaultValue::Entry(rhs);
  return *this;
}

bool TestEnumDefaultValue::operator==(UnderlyingType rhs) const {
  return mValue == TestEnumDefaultValue::Entry(rhs);
}

bool TestEnumDefaultValue::operator==(const TestEnumDefaultValue &rhs) const {
  return mValue == rhs.mValue;
}

bool TestEnumDefaultValue::operator!=(UnderlyingType rhs) const {
  return mValue != TestEnumDefaultValue::Entry(rhs);
}

bool TestEnumDefaultValue::operator!=(const TestEnumDefaultValue &rhs) const {
  return mValue != rhs.mValue;
}

bool TestEnumDefaultValue::operator<(UnderlyingType rhs) const {
  return mValue < TestEnumDefaultValue::Entry(rhs);
}

bool TestEnumDefaultValue::operator<(const TestEnumDefaultValue &rhs) const {
  return mValue < rhs.mValue;
}

bool TestEnumDefaultValue::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsString()) {
    *this = TestEnumDefaultValue::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestEnumDefaultValue(json.GetInt64());
  } else {
    *this = TestEnumDefaultValue{};
    HOLGEN_WARN("Could not json-parse TestEnumDefaultValue enum: invalid json type");
    return false;
  }
  return true;
}

rapidjson::Value TestEnumDefaultValue::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  return rapidjson::Value(GetValue());
}

void TestEnumDefaultValue::PushToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestEnumDefaultValue::UnderlyingType(mValue), luaState);
}

void TestEnumDefaultValue::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestEnumDefaultValue::UnderlyingType(mValue), luaState);
}

TestEnumDefaultValue TestEnumDefaultValue::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestEnumDefaultValue(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing TestEnumDefaultValue: {}", lua_typename(luaState, typ));
    return TestEnumDefaultValue{TestEnumDefaultValue::Invalid};
  }
}

void TestEnumDefaultValue::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "SomeEntry");
  lua_pushnumber(luaState, TestEnumDefaultValue::SomeEntry);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "DefaultEntry");
  lua_pushnumber(luaState, TestEnumDefaultValue::DefaultEntry);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "AnotherEntry");
  lua_pushnumber(luaState, TestEnumDefaultValue::AnotherEntry);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 3);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "SomeEntry");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "DefaultEntry");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "AnotherEntry");
  lua_rawseti(luaState, -2, 2);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 3);
  lua_setglobal(luaState, "TestEnumDefaultValue");
}
}
