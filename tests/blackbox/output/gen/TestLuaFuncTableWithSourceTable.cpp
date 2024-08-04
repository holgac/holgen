// This file is generated by holgen. Do not modify manually.
#include "TestLuaFuncTableWithSourceTable.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestLuaFuncTableContainer.h"

namespace holgen_blackbox_test {
bool TestLuaFuncTableWithSourceTable::operator==(const TestLuaFuncTableWithSourceTable &rhs) const {
  return true;
}

void TestLuaFuncTableWithSourceTable::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestLuaFuncTableWithSourceTable::GetTable() {
  return mTable;
}

void TestLuaFuncTableWithSourceTable::SetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset SetField function from table");
  lua_getglobal(luaState, "Scripts");
  lua_pushstring(luaState, mTable.c_str());
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table Scripts.{} not found when calling SetField", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "SetField");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined SetField function in Scripts.{}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  LuaHelper::Push(*this, luaState, false);
  LuaHelper::Push(container, luaState, false);
  lua_call(luaState, 2, 0);
  lua_pop(luaState, 2);
}

int32_t TestLuaFuncTableWithSourceTable::GetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), {}, "Calling unset GetField function from table");
  lua_getglobal(luaState, "Scripts");
  lua_pushstring(luaState, mTable.c_str());
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table Scripts.{} not found when calling GetField", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_pushstring(luaState, "GetField");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined GetField function in Scripts.{}", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push(*this, luaState, false);
  LuaHelper::Push(container, luaState, false);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 3);
  return result;
}

bool TestLuaFuncTableWithSourceTable::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return JsonHelper::Parse(mTable, json, converter);
}

void TestLuaFuncTableWithSourceTable::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaFuncTableWithSourceTableMeta");
  lua_setmetatable(luaState, -2);
}

void TestLuaFuncTableWithSourceTable::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
}

void TestLuaFuncTableWithSourceTable::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaFuncTableWithSourceTable *TestLuaFuncTableWithSourceTable::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaFuncTableWithSourceTable *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaFuncTableWithSourceTable TestLuaFuncTableWithSourceTable::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  return TestLuaFuncTableWithSourceTable{};
}

int TestLuaFuncTableWithSourceTable::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestLuaFuncTableWithSourceTable::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaFuncTableWithSourceTable::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaFuncTableWithSourceTable::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaFuncTableWithSourceTableMeta");
}

int TestLuaFuncTableWithSourceTable::SetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTableWithSourceTable::ReadProxyFromLua(luaState, -2);
  TestLuaFuncTableContainer arg0Mirror;
  TestLuaFuncTableContainer *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestLuaFuncTableContainer::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  instance->SetField(luaState, *arg0);
  return 0;
}

int TestLuaFuncTableWithSourceTable::GetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTableWithSourceTable::ReadProxyFromLua(luaState, -2);
  TestLuaFuncTableContainer arg0Mirror;
  TestLuaFuncTableContainer *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestLuaFuncTableContainer::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->GetField(luaState, *arg0);
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestLuaFuncTableWithSourceTable::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("SetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTableWithSourceTable::SetFieldCallerFromLua);
  } else if (0 == strcmp("GetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTableWithSourceTable::GetFieldCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaFuncTableWithSourceTable.{}", key);
    return 0;
  }
  return 1;
}
}
