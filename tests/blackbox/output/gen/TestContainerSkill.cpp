// This file is generated by holgen. Do not modify manually.
#include "TestContainerSkill.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestContainerSkill::TestContainerSkill(TestContainerSkill::UnderlyingType value) :
    mValue(TestContainerSkill::Entry(value)) {
}

TestContainerSkill::TestContainerSkill(Entry value) :
    mValue(value) {
}

TestContainerSkill::Entry TestContainerSkill::GetValue() const {
  return Entry(mValue);
}

TestContainerSkill TestContainerSkill::FromString(std::string_view str) {
  if (str == "Strength") {
    return TestContainerSkill(TestContainerSkill::Strength);
  } else if (str == "Perception") {
    return TestContainerSkill(TestContainerSkill::Perception);
  } else if (str == "Endurance") {
    return TestContainerSkill(TestContainerSkill::Endurance);
  } else if (str == "Charisma") {
    return TestContainerSkill(TestContainerSkill::Charisma);
  } else {
    HOLGEN_WARN("{} is not a valid TestContainerSkill, returning invalid", str);
    return TestContainerSkill{TestContainerSkill::Invalid};
  }
}

const char *TestContainerSkill::ToString() const {
  switch (mValue) {
    case TestContainerSkill::Strength:
      return "Strength";
    case TestContainerSkill::Perception:
      return "Perception";
    case TestContainerSkill::Endurance:
      return "Endurance";
    case TestContainerSkill::Charisma:
      return "Charisma";
    default:
      return "INVALID";
  }
}

TestContainerSkill &TestContainerSkill::operator=(UnderlyingType rhs) {
  mValue = TestContainerSkill::Entry(rhs);
  return *this;
}

bool TestContainerSkill::operator==(UnderlyingType rhs) const {
  return mValue == TestContainerSkill::Entry(rhs);
}

bool TestContainerSkill::operator==(const TestContainerSkill &rhs) const {
  return mValue == rhs.mValue;
}

bool TestContainerSkill::operator!=(UnderlyingType rhs) const {
  return mValue != TestContainerSkill::Entry(rhs);
}

bool TestContainerSkill::operator!=(const TestContainerSkill &rhs) const {
  return mValue != rhs.mValue;
}

bool TestContainerSkill::operator<(UnderlyingType rhs) const {
  return mValue < TestContainerSkill::Entry(rhs);
}

bool TestContainerSkill::operator<(const TestContainerSkill &rhs) const {
  return mValue < rhs.mValue;
}

bool TestContainerSkill::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsString()) {
    *this = TestContainerSkill::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestContainerSkill(json.GetInt64());
  } else {
    *this = TestContainerSkill{};
    HOLGEN_WARN("Could not json-parse TestContainerSkill enum: invalid json type");
    return false;
  }
  return true;
}

rapidjson::Value TestContainerSkill::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  return rapidjson::Value(GetValue());
}

void TestContainerSkill::PushToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestContainerSkill::UnderlyingType(mValue), luaState);
}

void TestContainerSkill::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push<true>(TestContainerSkill::UnderlyingType(mValue), luaState);
}

TestContainerSkill TestContainerSkill::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return TestContainerSkill(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing TestContainerSkill: {}", lua_typename(luaState, typ));
    return TestContainerSkill{};
  }
}

void TestContainerSkill::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Strength");
  lua_pushnumber(luaState, TestContainerSkill::Strength);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Perception");
  lua_pushnumber(luaState, TestContainerSkill::Perception);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Endurance");
  lua_pushnumber(luaState, TestContainerSkill::Endurance);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Charisma");
  lua_pushnumber(luaState, TestContainerSkill::Charisma);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 4);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Strength");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "Perception");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Endurance");
  lua_rawseti(luaState, -2, 2);
  lua_pushstring(luaState, "Charisma");
  lua_rawseti(luaState, -2, 3);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 4);
  lua_setglobal(luaState, "TestContainerSkill");
}
}
