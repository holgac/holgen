// This file is generated by holgen. Do not modify manually.
#include "TestContainerInnerStructWithId.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerInnerStructWithId::operator==(const TestContainerInnerStructWithId &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName
  );
}

uint32_t TestContainerInnerStructWithId::GetId() const {
  return mId;
}

const std::string &TestContainerInnerStructWithId::GetName() const {
  return mName;
}

std::string &TestContainerInnerStructWithId::GetName() {
  return mName;
}

void TestContainerInnerStructWithId::SetId(uint32_t val) {
  mId = val;
}

void TestContainerInnerStructWithId::SetName(const std::string &val) {
  mName = val;
}

bool TestContainerInnerStructWithId::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructWithId.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructWithId.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestContainerInnerStructWithId: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerInnerStructWithId!");
      auto res = JsonHelper::Parse(mId, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructWithId.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerInnerStructWithId!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructWithId.name field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestContainerInnerStructWithId!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestContainerInnerStructWithId.");
    return false;
  }
  return true;
}

void TestContainerInnerStructWithId::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerInnerStructWithIdMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerInnerStructWithId::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
}

void TestContainerInnerStructWithId::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerInnerStructWithId *TestContainerInnerStructWithId::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestContainerInnerStructWithId", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerInnerStructWithId *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerInnerStructWithId TestContainerInnerStructWithId::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerInnerStructWithId{};
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
      HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructWithId.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerInnerStructWithId::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerInnerStructWithId::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerInnerStructWithId.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerInnerStructWithId.name from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructWithId.{}", key);
  }
  return 0;
}

void TestContainerInnerStructWithId::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerInnerStructWithId::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerInnerStructWithId::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerInnerStructWithIdMeta");
}

int TestContainerInnerStructWithId::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = TestContainerInnerStructWithId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerInnerStructWithId.id with an invalid lua proxy object!");
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = TestContainerInnerStructWithId::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerInnerStructWithId.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructWithId.{}", key);
    return 0;
  }
  return 1;
}
}
