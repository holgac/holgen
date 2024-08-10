// This file is generated by holgen. Do not modify manually.
#include "TestLuaFuncTableStatic.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestLuaFuncTableContainer.h"

namespace holgen_blackbox_test {
bool TestLuaFuncTableStatic::operator==(const TestLuaFuncTableStatic &rhs) const {
  return true;
}

void TestLuaFuncTableStatic::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestLuaFuncTableStatic::GetTable() {
  return mTable;
}

void TestLuaFuncTableStatic::SetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
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
  LuaHelper::Push(container, luaState, false);
  lua_call(luaState, 1, 0);
  lua_pop(luaState, 1);
}

int32_t TestLuaFuncTableStatic::GetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
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
  LuaHelper::Push(container, luaState, false);
  lua_call(luaState, 1, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 2);
  return result;
}

bool TestLuaFuncTableStatic::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return JsonHelper::Parse(mTable, json, converter);
}

void TestLuaFuncTableStatic::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaFuncTableStaticMeta");
  lua_setmetatable(luaState, -2);
}

void TestLuaFuncTableStatic::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
}

void TestLuaFuncTableStatic::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaFuncTableStatic *TestLuaFuncTableStatic::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaFuncTableStatic *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaFuncTableStatic TestLuaFuncTableStatic::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  return TestLuaFuncTableStatic{};
}

int TestLuaFuncTableStatic::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestLuaFuncTableStatic::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaFuncTableStatic::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaFuncTableStatic::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaFuncTableStaticMeta");
}

int TestLuaFuncTableStatic::SetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTableStatic::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaFuncTableStatic.SetField method with an invalid lua proxy object!");
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

int TestLuaFuncTableStatic::GetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTableStatic::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaFuncTableStatic.GetField method with an invalid lua proxy object!");
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

int TestLuaFuncTableStatic::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("SetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTableStatic::SetFieldCallerFromLua);
  } else if (0 == strcmp("GetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTableStatic::GetFieldCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaFuncTableStatic.{}", key);
    return 0;
  }
  return 1;
}
}
