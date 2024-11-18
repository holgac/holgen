// This file is generated by holgen. Do not modify manually.
#include "TestLuaFuncTable.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestLuaFuncTableContainer.h"

namespace holgen_blackbox_test {
void TestLuaFuncTable::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestLuaFuncTable::GetTable() const {
  return mTable;
}

void TestLuaFuncTable::SetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
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
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(container, luaState);
  lua_call(luaState, 2, 0);
  lua_pop(luaState, 1);
}

int32_t TestLuaFuncTable::GetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const {
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
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(container, luaState);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 2);
  return result;
}

bool TestLuaFuncTable::operator==(const TestLuaFuncTable &rhs) const {
  return !(
      mTable != rhs.mTable
  );
}

bool TestLuaFuncTable::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return JsonHelper::Parse(mTable, json, converter);
}

rapidjson::Value TestLuaFuncTable::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  return val;
}

void TestLuaFuncTable::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaFuncTable");
  lua_setmetatable(luaState, -2);
}

void TestLuaFuncTable::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
}

void TestLuaFuncTable::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaFuncTable *TestLuaFuncTable::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestLuaFuncTable", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaFuncTable *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaFuncTable TestLuaFuncTable::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  return TestLuaFuncTable{};
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
  lua_setglobal(luaState, "TestLuaFuncTable");
}

int TestLuaFuncTable::SetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTable::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaFuncTable.SetField method with an invalid lua proxy object!");
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

int TestLuaFuncTable::GetFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaFuncTable::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaFuncTable.GetField method with an invalid lua proxy object!");
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
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaFuncTable::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("SetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTable::SetFieldCallerFromLua);
  } else if (0 == strcmp("GetField", key)) {
    lua_pushcfunction(luaState, TestLuaFuncTable::GetFieldCallerFromLua);
  } else if (0 == strcmp("TABLE", key)) {
    auto instance = TestLuaFuncTable::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push<false>(instance->mTable, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaFuncTable.{}", key);
    return 0;
  }
  return 1;
}
}
