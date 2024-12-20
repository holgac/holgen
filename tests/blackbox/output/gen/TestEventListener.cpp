// This file is generated by holgen. Do not modify manually.
#include "TestEventListener.h"

#include <cstring>
#include <lua.hpp>
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::string &TestEventListener::GetName() const {
  return mName;
}

std::string &TestEventListener::GetName() {
  return mName;
}

void TestEventListener::SetName(const std::string &val) {
  mName = val;
}

void TestEventListener::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestEventListener::GetTable() const {
  return mTable;
}

void TestEventListener::OnEvent1(lua_State *luaState, const int val) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset OnEvent1 function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling OnEvent1", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "OnEvent1");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined OnEvent1 function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_copy(luaState, -1, -2);
  lua_pop(luaState, 1);
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(val, luaState);
  lua_call(luaState, 2, 0);
}

int TestEventListener::OnEvent2(lua_State *luaState) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), {}, "Calling unset OnEvent2 function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling OnEvent2", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_pushstring(luaState, "OnEvent2");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined OnEvent2 function in {}", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_copy(luaState, -1, -2);
  lua_pop(luaState, 1);
  LuaHelper::Push<false>(*this, luaState);
  lua_call(luaState, 1, 1);
  int result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

bool TestEventListener::operator==(const TestEventListener &rhs) const {
  return !(
      mName != rhs.mName ||
      mTable != rhs.mTable
  );
}

void TestEventListener::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestEventListener");
  lua_setmetatable(luaState, -2);
}

void TestEventListener::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
}

void TestEventListener::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestEventListener *TestEventListener::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestEventListener", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestEventListener *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestEventListener TestEventListener::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestEventListener{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestEventListener.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestEventListener::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestEventListener::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestEventListener.name from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEventListener.{}", key);
  }
  return 0;
}

int TestEventListener::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestEventListener::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestEventListener.operator== method with an invalid lua proxy object!");
  TestEventListener arg0Mirror;
  TestEventListener *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestEventListener::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestEventListener::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestEventListener::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestEventListener::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestEventListener::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestEventListener::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestEventListener");
}

int TestEventListener::OnEvent1CallerFromLua(lua_State *luaState) {
  auto instance = TestEventListener::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestEventListener.OnEvent1 method with an invalid lua proxy object!");
  int arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->OnEvent1(luaState, arg0);
  return 0;
}

int TestEventListener::OnEvent2CallerFromLua(lua_State *luaState) {
  auto instance = TestEventListener::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestEventListener.OnEvent2 method with an invalid lua proxy object!");
  auto result = instance->OnEvent2(luaState);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestEventListener::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("name", key)) {
    auto instance = TestEventListener::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestEventListener.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("OnEvent1", key)) {
    lua_pushcfunction(luaState, TestEventListener::OnEvent1CallerFromLua);
  } else if (0 == strcmp("OnEvent2", key)) {
    lua_pushcfunction(luaState, TestEventListener::OnEvent2CallerFromLua);
  } else if (0 == strcmp("TABLE", key)) {
    auto instance = TestEventListener::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push<false>(instance->mTable, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEventListener.{}", key);
    return 0;
  }
  return 1;
}
}
