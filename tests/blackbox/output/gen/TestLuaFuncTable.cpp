// This file is generated by holgen. Do not modify manually.
#include "TestLuaFuncTable.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestLuaFuncTableContainer.h"

namespace holgen_blackbox_test {
bool TestLuaFuncTable::operator==(const TestLuaFuncTable &rhs) const {
  return true;
}

void TestLuaFuncTable::SetTable(std::string val) {
  mTable = std::move(val);
}

void TestLuaFuncTable::SetField(lua_State *luaState, const TestLuaFuncTableContainer *container) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset SetField function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling SetField", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "SetField");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined SetField function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  LuaHelper::Push(*this, luaState);
  LuaHelper::Push(container, luaState);
  lua_call(luaState, 2, 0);
  lua_pop(luaState, 1);
}

int32_t TestLuaFuncTable::GetField(lua_State *luaState, const TestLuaFuncTableContainer *container) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), {}, "Calling unset GetField function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling GetField", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_pushstring(luaState, "GetField");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined GetField function in {}", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push(*this, luaState);
  LuaHelper::Push(container, luaState);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 2);
  return result;
}

bool TestLuaFuncTable::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return JsonHelper::Parse(mTable, json, converter);
}

void TestLuaFuncTable::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaFuncTableMeta");
  lua_setmetatable(luaState, -2);
}

void TestLuaFuncTable::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaFuncTable *TestLuaFuncTable::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaFuncTable *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestLuaFuncTable::IndexMetaMethod(lua_State *luaState) {
  return 0;
}

int TestLuaFuncTable::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestLuaFuncTable::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaFuncTable::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaFuncTable::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaFuncTableMeta");
}
}
