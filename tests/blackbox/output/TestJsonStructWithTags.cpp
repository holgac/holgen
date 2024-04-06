// This file is generated by holgen. Do not modify manually.
#include "TestJsonStructWithTags.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<uint64_t>& TestJsonStructWithTags::GetTags() const {
  return mTags;
}
std::vector<uint64_t>& TestJsonStructWithTags::GetTags() {
  return mTags;
}
void TestJsonStructWithTags::SetTags(const std::vector<uint64_t>& val) {
  mTags = val;
}
bool TestJsonStructWithTags::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsObject()) {
    for(const auto& data: json.GetObject()) {
      const auto& name = data.name.GetString();
      if (0 == strcmp("tags", name)) {
        auto res = JsonHelper::Parse<std::string>(mTags, data.value, converter, converter.testJsonConvertTag);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructWithTags.tags field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonStructWithTags: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse<std::string>(mTags, json, converter, converter.testJsonConvertTag);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructWithTags.tags field");
  }
  return true;
}
void TestJsonStructWithTags::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonStructWithTagsMeta");
  lua_setmetatable(luaState, -2);
}
void TestJsonStructWithTags::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestJsonStructWithTags* TestJsonStructWithTags::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonStructWithTags*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestJsonStructWithTags::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestJsonStructWithTags::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("tags", key)) {
      LuaHelper::Push(instance->mTags, ls);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonStructWithTags.{}", key);
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void TestJsonStructWithTags::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestJsonStructWithTags::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("tags", key)) {
      LuaHelper::Read(instance->mTags, ls, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonStructWithTags.{}", key);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestJsonStructWithTags::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestJsonStructWithTagsMeta");
}
}
