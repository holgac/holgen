// This file is generated by holgen. Do not modify manually.
#include "TestEnum.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestEnum::TestEnum(TestEnum::UnderlyingType value) :
    mValue(TestEnum::Entry(value)) {
}

TestEnum::TestEnum(Entry value) :
    mValue(value) {
}

TestEnum::Entry TestEnum::GetValue() const {
  return Entry(mValue);
}

TestEnum TestEnum::FromString(std::string_view str) {
  if (str == "Entry5") {
    return TestEnum(TestEnum::Entry5);
  } else if (str == "Entry1") {
    return TestEnum(TestEnum::Entry1);
  } else if (str == "Entry2") {
    return TestEnum(TestEnum::Entry2);
  } else {
    HOLGEN_WARN("{} is not a valid TestEnum, returning invalid", str);
    return TestEnum{TestEnum::Invalid};
  }
}

const char *TestEnum::ToString() const {
  switch (mValue) {
    case TestEnum::Entry5:
      return "Entry5";
    case TestEnum::Entry1:
      return "Entry1";
    case TestEnum::Entry2:
      return "Entry2";
    default:
      return "INVALID";
  }
}

TestEnum &TestEnum::operator=(UnderlyingType rhs) {
  mValue = TestEnum::Entry(rhs);
  return *this;
}

bool TestEnum::operator==(UnderlyingType rhs) const {
  return mValue == TestEnum::Entry(rhs);
}

bool TestEnum::operator==(const TestEnum &rhs) const {
  return mValue == rhs.mValue;
}

bool TestEnum::operator!=(UnderlyingType rhs) const {
  return mValue != TestEnum::Entry(rhs);
}

bool TestEnum::operator!=(const TestEnum &rhs) const {
  return mValue != rhs.mValue;
}

bool TestEnum::operator<(UnderlyingType rhs) const {
  return mValue < TestEnum::Entry(rhs);
}

bool TestEnum::operator<(const TestEnum &rhs) const {
  return mValue < rhs.mValue;
}

bool TestEnum::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsString()) {
    *this = TestEnum::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestEnum(json.GetInt64());
  } else {
    *this = TestEnum{};
    HOLGEN_WARN("Could not json-parse TestEnum enum: invalid json type");
    return false;
  }
  return true;
}

rapidjson::Value TestEnum::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  return rapidjson::Value(GetValue());
}

void TestEnum::PushToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestEnum::UnderlyingType(mValue), luaState);
}

void TestEnum::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestEnum::UnderlyingType(mValue), luaState);
}

TestEnum TestEnum::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestEnum(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing TestEnum: {}", lua_typename(luaState, typ));
    return TestEnum{};
  }
}

void TestEnum::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Entry5");
  lua_pushnumber(luaState, TestEnum::Entry5);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry1");
  lua_pushnumber(luaState, TestEnum::Entry1);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry2");
  lua_pushnumber(luaState, TestEnum::Entry2);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 2);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry5");
  lua_rawseti(luaState, -2, 5);
  lua_pushstring(luaState, "Entry1");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "Entry2");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 2);
  lua_setglobal(luaState, "TestEnum");
}
}
