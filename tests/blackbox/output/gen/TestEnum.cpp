// This file is generated by holgen. Do not modify manually.
#include "TestEnum.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
TestEnum::TestEnum(TestEnum::UnderlyingType value) :
  mValue(value)
{
}

TestEnum::TestEnum(Entry value) :
  mValue(UnderlyingType(value))
{
}
TestEnum::Entry TestEnum::GetValue() const {
  return Entry(mValue);
}

TestEnum TestEnum::FromString(std::string_view str) {
  if (str == "Entry5") {
    return TestEnum(5);
  } else if (str == "Entry1") {
    return TestEnum(0);
  } else if (str == "Entry2") {
    return TestEnum(1);
  } else {
    return TestEnum(TestEnum::Invalid);
  }
}

const char* TestEnum::ToString() const {
  switch (mValue) {
    case 5: return "Entry5";
    case 0: return "Entry1";
    case 1: return "Entry2";
    default: return "INVALID";
  }
}

TestEnum& TestEnum::operator =(UnderlyingType rhs) {
  mValue = rhs;
  return *this;
}

bool TestEnum::operator ==(UnderlyingType rhs) const {
  return mValue == rhs;
}

bool TestEnum::operator ==(const TestEnum& rhs) const {
  return mValue == rhs.mValue;
}

bool TestEnum::operator !=(UnderlyingType rhs) const {
  return mValue != rhs;
}

bool TestEnum::operator !=(const TestEnum& rhs) const {
  return mValue != rhs.mValue;
}

bool TestEnum::operator <(UnderlyingType rhs) const {
  return mValue < rhs;
}

bool TestEnum::operator <(const TestEnum& rhs) const {
  return mValue < rhs.mValue;
}

bool TestEnum::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsString()) {
    *this = TestEnum::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestEnum(json.GetInt64());
  } else {
    *this = TestEnum(TestEnum::Invalid);
    HOLGEN_WARN("Could not json-parse TestEnum enum: invalid json type");
    return false;
  }
  return true;
}

void TestEnum::PushToLua(lua_State* luaState) const {
  LuaHelper::Push(mValue, luaState);
}

TestEnum TestEnum::ReadFromLua(lua_State* luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestEnum(lua_tonumber(luaState, idx));
  } else {
    return TestEnum{Invalid};
  }
}

void TestEnum::PushEnumToLua(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Entry5");
  lua_pushnumber(luaState, 5);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry1");
  lua_pushnumber(luaState, 0);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Entry2");
  lua_pushnumber(luaState, 1);
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
