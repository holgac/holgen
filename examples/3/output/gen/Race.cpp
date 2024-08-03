// This file is generated by holgen. Do not modify manually.
#include "Race.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
Race::Race(Race::UnderlyingType value) :
    mValue(Race::Entry(value)) {
}

Race::Race(Entry value) :
    mValue(value) {
}

Race::Entry Race::GetValue() const {
  return Entry(mValue);
}

Race Race::FromString(std::string_view str) {
  if (str == "Human") {
    return Race(Race::Human);
  } else if (str == "Elf") {
    return Race(Race::Elf);
  } else if (str == "Dwarf") {
    return Race(Race::Dwarf);
  } else if (str == "Hobbit") {
    return Race(Race::Hobbit);
  } else {
    HOLGEN_WARN("{} is not a valid Race, returning invalid", str);
    return Race{Race::Invalid};
  }
}

const char *Race::ToString() const {
  switch (mValue) {
    case Race::Human:
      return "Human";
    case Race::Elf:
      return "Elf";
    case Race::Dwarf:
      return "Dwarf";
    case Race::Hobbit:
      return "Hobbit";
    default:
      return "INVALID";
  }
}

Race &Race::operator=(UnderlyingType rhs) {
  mValue = Race::Entry(rhs);
  return *this;
}

bool Race::operator==(UnderlyingType rhs) const {
  return mValue == Race::Entry(rhs);
}

bool Race::operator==(const Race &rhs) const {
  return mValue == rhs.mValue;
}

bool Race::operator!=(UnderlyingType rhs) const {
  return mValue != Race::Entry(rhs);
}

bool Race::operator!=(const Race &rhs) const {
  return mValue != rhs.mValue;
}

bool Race::operator<(UnderlyingType rhs) const {
  return mValue < Race::Entry(rhs);
}

bool Race::operator<(const Race &rhs) const {
  return mValue < rhs.mValue;
}

bool Race::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsString()) {
    *this = Race::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = Race(json.GetInt64());
  } else {
    *this = Race{};
    HOLGEN_WARN("Could not json-parse Race enum: invalid json type");
    return false;
  }
  return true;
}

void Race::PushToLua(lua_State *luaState) const {
  LuaHelper::Push(Race::UnderlyingType(mValue), luaState, true);
}

void Race::PushMirrorToLua(lua_State *luaState) const {
  LuaHelper::Push(Race::UnderlyingType(mValue), luaState, true);
}

Race Race::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto typ = lua_type(luaState, idx);
  if (typ == LUA_TSTRING) {
    return FromString(lua_tostring(luaState, idx));
  } else if (typ == LUA_TNUMBER) {
    return Race(lua_tonumber(luaState, idx));
  } else {
    HOLGEN_WARN("Unexpected type when parsing Race: {}", lua_typename(luaState, typ));
    return Race{};
  }
}

void Race::PushEnumToLua(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "Human");
  lua_pushnumber(luaState, Race::Human);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Elf");
  lua_pushnumber(luaState, Race::Elf);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Dwarf");
  lua_pushnumber(luaState, Race::Dwarf);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Hobbit");
  lua_pushnumber(luaState, Race::Hobbit);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Invalid");
  lua_pushnumber(luaState, 4);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Human");
  lua_rawseti(luaState, -2, 2);
  lua_pushstring(luaState, "Elf");
  lua_rawseti(luaState, -2, 0);
  lua_pushstring(luaState, "Dwarf");
  lua_rawseti(luaState, -2, 1);
  lua_pushstring(luaState, "Hobbit");
  lua_rawseti(luaState, -2, 3);
  lua_pushstring(luaState, "Invalid");
  lua_rawseti(luaState, -2, 4);
  lua_setglobal(luaState, "Race");
}
}
