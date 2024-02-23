#include "LuaHelper.h"

namespace holgen_blackbox_test {
void LuaHelper::Push(bool data, lua_State* luaState) {
  lua_pushboolean(luaState, data);
}
void LuaHelper::Push(double data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(float data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(int16_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(int32_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(int64_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(int8_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(const std::string& data, lua_State* luaState) {
  lua_pushstring(luaState, data.c_str());
}
void LuaHelper::Push(uint16_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(uint32_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(uint64_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
void LuaHelper::Push(uint8_t data, lua_State* luaState) {
  lua_pushnumber(luaState, data);
}
bool LuaHelper::Read(bool& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isboolean(luaState, luaIndex))
    return false;
  data = lua_toboolean(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(double& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(float& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(int16_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(int32_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(int64_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(int8_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(std::string& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isstring(luaState, luaIndex))
    return false;
  data = lua_tostring(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(uint16_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(uint32_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(uint64_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
bool LuaHelper::Read(uint8_t& data, lua_State* luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}
}
