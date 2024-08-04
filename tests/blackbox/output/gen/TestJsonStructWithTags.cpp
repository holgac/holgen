// This file is generated by holgen. Do not modify manually.
#include "TestJsonStructWithTags.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestJsonStructWithTags::operator==(const TestJsonStructWithTags &rhs) const {
  return !(
      mTags != rhs.mTags
  );
}

const std::vector<uint64_t> &TestJsonStructWithTags::GetTags() const {
  return mTags;
}

std::vector<uint64_t> &TestJsonStructWithTags::GetTags() {
  return mTags;
}

void TestJsonStructWithTags::SetTags(const std::vector<uint64_t> &val) {
  mTags = val;
}

bool TestJsonStructWithTags::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("tags", name)) {
        auto res = JsonHelper::ParseConvertElem<std::string>(mTags, data.value, converter, converter.testJsonConvertTag);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructWithTags.tags field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonStructWithTags: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::ParseConvertElem<std::string>(mTags, json, converter, converter.testJsonConvertTag);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructWithTags.tags field");
  }
  return true;
}

void TestJsonStructWithTags::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonStructWithTagsMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonStructWithTags::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "tags");
  LuaHelper::Push(mTags, luaState, true);
  lua_settable(luaState, -3);
}

void TestJsonStructWithTags::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonStructWithTags *TestJsonStructWithTags::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonStructWithTags *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonStructWithTags TestJsonStructWithTags::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonStructWithTags{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("tags", key)) {
      LuaHelper::Read(result.mTags, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonStructWithTags.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonStructWithTags::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructWithTags::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("tags", key)) {
    LuaHelper::Read(instance->mTags, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructWithTags.{}", key);
  }
  return 0;
}

void TestJsonStructWithTags::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonStructWithTags::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonStructWithTags::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonStructWithTagsMeta");
}

int TestJsonStructWithTags::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructWithTags::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructWithTags.{} with an invalid lua proxy object!", key);
  if (0 == strcmp("tags", key)) {
    LuaHelper::Push(instance->mTags, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructWithTags.{}", key);
    return 0;
  }
  return 1;
}
}
