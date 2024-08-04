// This file is generated by holgen. Do not modify manually.
#include "TestLuaRegistryData.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestLuaRegistryData::TestLuaRegistryData(TestLuaRegistryData &&rhs) {
  std::swap(mData, rhs.mData);
  mTable = std::move(rhs.mTable);
}

TestLuaRegistryData::~TestLuaRegistryData() {
  HOLGEN_WARN_IF(mData != LUA_NOREF, "TestLuaRegistryData.data was not released!");
}

void TestLuaRegistryData::InitializeLua(lua_State *luaState) {
  HOLGEN_WARN_IF(mData != LUA_NOREF, "TestLuaRegistryData.data was already initialized!");
  lua_newtable(luaState);
  mData = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

void TestLuaRegistryData::UninitializeLua(lua_State *luaState) {
  luaL_unref(luaState, LUA_REGISTRYINDEX, mData);
  mData = LUA_NOREF;
}

bool TestLuaRegistryData::operator==(const TestLuaRegistryData &rhs) const {
  return !(
      mData != rhs.mData
  );
}

int TestLuaRegistryData::GetData() const {
  return mData;
}

void TestLuaRegistryData::SetData(int val) {
  mData = val;
}

void TestLuaRegistryData::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestLuaRegistryData::GetTable() {
  return mTable;
}

void TestLuaRegistryData::Init(lua_State *luaState, const std::function<void(lua_State *, const TestLuaRegistryData &)> &initData) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset Init function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Init", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "Init");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Init function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  LuaHelper::Push(*this, luaState, false);
  initData(luaState, *this);
  lua_call(luaState, 2, 0);
  lua_pop(luaState, 1);
}

int32_t TestLuaRegistryData::Get(lua_State *luaState) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), {}, "Calling unset Get function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Get", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_pushstring(luaState, "Get");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Get function in {}", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push(*this, luaState, false);
  lua_call(luaState, 1, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 2);
  return result;
}

void TestLuaRegistryData::Add(lua_State *luaState, const int32_t val) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset Add function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Add", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "Add");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Add function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  LuaHelper::Push(*this, luaState, false);
  LuaHelper::Push(val, luaState, false);
  lua_call(luaState, 2, 0);
  lua_pop(luaState, 1);
}

bool TestLuaRegistryData::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return JsonHelper::Parse(mTable, json, converter);
}

void TestLuaRegistryData::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaRegistryDataMeta");
  lua_setmetatable(luaState, -2);
}

void TestLuaRegistryData::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "data");
  lua_rawgeti(luaState, LUA_REGISTRYINDEX, mData);
  lua_settable(luaState, -3);
}

void TestLuaRegistryData::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaRegistryData *TestLuaRegistryData::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaRegistryData *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaRegistryData TestLuaRegistryData::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestLuaRegistryData{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("data", key)) {
      result.mData = luaL_ref(luaState, LUA_REGISTRYINDEX);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestLuaRegistryData.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestLuaRegistryData::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestLuaRegistryData::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaRegistryData::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaRegistryData::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaRegistryDataMeta");
}

int TestLuaRegistryData::InitCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
  std::function<void(lua_State *, const TestLuaRegistryData &)> arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->Init(luaState, arg0);
  return 0;
}

int TestLuaRegistryData::GetCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -1);
  auto result = instance->Get(luaState);
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestLuaRegistryData::AddCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->Add(luaState, arg0);
  return 0;
}

int TestLuaRegistryData::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("data", key)) {
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, instance->mData);
  } else if (0 == strcmp("Init", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::InitCallerFromLua);
  } else if (0 == strcmp("Get", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::GetCallerFromLua);
  } else if (0 == strcmp("Add", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::AddCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaRegistryData.{}", key);
    return 0;
  }
  return 1;
}

TestLuaRegistryData &TestLuaRegistryData::operator=(TestLuaRegistryData &&rhs) {
  std::swap(mData, rhs.mData);
  mTable = std::move(rhs.mTable);
  return *this;
}
}
