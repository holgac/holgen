// This file is generated by holgen. Do not modify manually.
#include "LuaHelper.h"

#include "Character.h"
#include "Race.h"
#include "WeaponInInventory.h"
#include "WeaponType.h"
#include "WeaponTypeBow.h"
#include "WeaponTypeSword.h"

namespace ex3_schemas {
void LuaHelper::Push(std::nullptr_t , lua_State *luaState, bool pushMirror) {
  lua_pushnil(luaState);
}

void LuaHelper::Push(bool data, lua_State *luaState, bool pushMirror) {
  lua_pushboolean(luaState, data);
}

void LuaHelper::Push(double data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(float data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int16_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int32_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int64_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int8_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(const std::string &data, lua_State *luaState, bool pushMirror) {
  lua_pushstring(luaState, data.c_str());
}

void LuaHelper::Push(uint16_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint32_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint64_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint8_t data, lua_State *luaState, bool pushMirror) {
  lua_pushnumber(luaState, data);
}

bool LuaHelper::Read(bool &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isboolean(luaState, luaIndex), false, "Reading from lua failed! Expected bool, got {}.", lua_typename(luaState, luaIndex));
  data = lua_toboolean(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(double &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected double, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(float &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected float, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int16_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int16_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int32_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int32_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int64_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int64_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int8_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int8_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(std::string &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isstring(luaState, luaIndex), false, "Reading from lua failed! Expected std::string, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tostring(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint16_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint16_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint32_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint32_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint64_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint64_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint8_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint8_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

void LuaHelper::CreateMetatables(lua_State *luaState) {
  Character::CreateLuaMetatable(luaState);
  WeaponInInventory::CreateLuaMetatable(luaState);
  WeaponTypeBow::CreateLuaMetatable(luaState);
  WeaponTypeSword::CreateLuaMetatable(luaState);
  Race::PushEnumToLua(luaState);
  WeaponType::PushEnumToLua(luaState);
}
}
