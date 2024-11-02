// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElemWithId.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t TestStructSingleElemWithId::GetId() const {
  return mId;
}

const std::string &TestStructSingleElemWithId::GetName() const {
  return mName;
}

std::string &TestStructSingleElemWithId::GetName() {
  return mName;
}

void TestStructSingleElemWithId::SetId(uint32_t val) {
  mId = val;
}

void TestStructSingleElemWithId::SetName(const std::string &val) {
  mName = val;
}

bool TestStructSingleElemWithId::operator==(const TestStructSingleElemWithId &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName
  );
}

bool TestStructSingleElemWithId::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemWithId.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructSingleElemWithId: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructSingleElemWithId!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemWithId.name field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructSingleElemWithId!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructSingleElemWithId.");
    return false;
  }
  return true;
}

void TestStructSingleElemWithId::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemWithIdMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructSingleElemWithId::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
}

void TestStructSingleElemWithId::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructSingleElemWithId *TestStructSingleElemWithId::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructSingleElemWithId", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElemWithId *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructSingleElemWithId TestStructSingleElemWithId::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructSingleElemWithId{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElemWithId.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructSingleElemWithId::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructSingleElemWithId::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructSingleElemWithId.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructSingleElemWithId.name from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemWithId.{}", key);
  }
  return 0;
}

void TestStructSingleElemWithId::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructSingleElemWithId::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructSingleElemWithId::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructSingleElemWithIdMeta");
}

int TestStructSingleElemWithId::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = TestStructSingleElemWithId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemWithId.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = TestStructSingleElemWithId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemWithId.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemWithId.{}", key);
    return 0;
  }
  return 1;
}
}
