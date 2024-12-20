// This file is generated by holgen. Do not modify manually.
#include "TestJsonEnum.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestJsonEnum::TestJsonEnum(TestJsonEnum::UnderlyingType value) :
    mValue(TestJsonEnum::Entry(value)) {
}

TestJsonEnum::TestJsonEnum(Entry value) :
    mValue(value) {
}

TestJsonEnum::Entry TestJsonEnum::GetValue() const {
  return Entry(mValue);
}

TestJsonEnum TestJsonEnum::FromString(std::string_view str) {
  if (str == "Entry1") {
    return TestJsonEnum(TestJsonEnum::Entry1);
  } else if (str == "Entry2") {
    return TestJsonEnum(TestJsonEnum::Entry2);
  } else if (str == "Entry5") {
    return TestJsonEnum(TestJsonEnum::Entry5);
  } else {
    HOLGEN_WARN("{} is not a valid TestJsonEnum, returning invalid", str);
    return TestJsonEnum{TestJsonEnum::Invalid};
  }
}

const char *TestJsonEnum::ToString() const {
  switch (mValue) {
    case TestJsonEnum::Entry1:
      return "Entry1";
    case TestJsonEnum::Entry2:
      return "Entry2";
    case TestJsonEnum::Entry5:
      return "Entry5";
    default:
      return "INVALID";
  }
}

TestJsonEnum &TestJsonEnum::operator=(UnderlyingType rhs) {
  mValue = TestJsonEnum::Entry(rhs);
  return *this;
}

bool TestJsonEnum::operator==(UnderlyingType rhs) const {
  return mValue == TestJsonEnum::Entry(rhs);
}

bool TestJsonEnum::operator==(const TestJsonEnum &rhs) const {
  return mValue == rhs.mValue;
}

bool TestJsonEnum::operator!=(UnderlyingType rhs) const {
  return mValue != TestJsonEnum::Entry(rhs);
}

bool TestJsonEnum::operator!=(const TestJsonEnum &rhs) const {
  return mValue != rhs.mValue;
}

bool TestJsonEnum::operator<(UnderlyingType rhs) const {
  return mValue < TestJsonEnum::Entry(rhs);
}

bool TestJsonEnum::operator<(const TestJsonEnum &rhs) const {
  return mValue < rhs.mValue;
}

bool TestJsonEnum::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsString()) {
    *this = TestJsonEnum::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestJsonEnum(json.GetInt64());
  } else {
    *this = TestJsonEnum{};
    HOLGEN_WARN("Could not json-parse TestJsonEnum enum: invalid json type");
    return false;
  }
  return true;
}

rapidjson::Value TestJsonEnum::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  return rapidjson::Value(GetValue());
}

void TestJsonEnum::PushToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestJsonEnum::UnderlyingType(mValue), luaState);
}

void TestJsonEnum::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestJsonEnum::UnderlyingType(mValue), luaState);
}

TestJsonEnum TestJsonEnum::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestJsonEnum(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing TestJsonEnum: {}", lua_typename(luaState, typ));
    return TestJsonEnum{};
  }
}

void TestJsonEnum::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Entry1");
  lua_pushnumber(luaState, TestJsonEnum::Entry1);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry2");
  lua_pushnumber(luaState, TestJsonEnum::Entry2);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry5");
  lua_pushnumber(luaState, TestJsonEnum::Entry5);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 0);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry1");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Entry2");
  lua_rawseti(luaState, -2, 2);
  lua_pushstring(luaState, "Entry5");
  lua_rawseti(luaState, -2, 5);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 0);
  lua_setglobal(luaState, "TestJsonEnum");
}
}
