// This file is generated by holgen. Do not modify manually.
#include "TestJsonTag.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestJsonTagManager.h"

namespace holgen_blackbox_test {
bool TestJsonTag::operator==(const TestJsonTag &rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName;
}

uint64_t TestJsonTag::GetId() const {
  return mId;
}

const std::string &TestJsonTag::GetName() const {
  return mName;
}

std::string &TestJsonTag::GetName() {
  return mName;
}

void TestJsonTag::SetId(uint64_t val) {
  mId = val;
}

void TestJsonTag::SetName(const std::string &val) {
  mName = val;
}

TestJsonTag *TestJsonTag::Get(uint64_t id) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTag(id);
}

TestJsonTag *TestJsonTag::GetFromName(const std::string &key) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTagFromName(key);
}

bool TestJsonTag::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonTag: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mName, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.name field");
  }
  return true;
}

void TestJsonTag::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonTagMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonTag::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState);
  lua_settable(luaState, -3);
}

void TestJsonTag::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonTag *TestJsonTag::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint64_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = TestJsonTag::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonTag TestJsonTag::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonTag{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonTag::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTag::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
    return 0;
  }
  return 1;
}

int TestJsonTag::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTag::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
  }
  return 0;
}

void TestJsonTag::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonTag::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonTag::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonTagMeta");
}
}
