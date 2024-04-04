// This file is generated by holgen. Do not modify manually.
#include "TestJsonTag.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "TestJsonTagManager.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint64_t TestJsonTag::GetId() const {
  return mId;
}
const std::string& TestJsonTag::GetName() const {
  return mName;
}
std::string& TestJsonTag::GetName() {
  return mName;
}
void TestJsonTag::SetId(uint64_t val) {
  mId = val;
}
void TestJsonTag::SetName(const std::string& val) {
  mName = val;
}
TestJsonTag* TestJsonTag::Get(uint64_t id) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTag(id);
}
TestJsonTag* TestJsonTag::GetFromName(const std::string& key) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTagFromName(key);
}
bool TestJsonTag::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsObject()) {
    for(const auto& data: json.GetObject()) {
      const auto& name = data.name.GetString();
      if (0 == strcmp(name, "id")) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.id field");
      } else if (0 == strcmp(name, "name")) {
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
void TestJsonTag::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonTagMeta");
  lua_setmetatable(luaState, -2);
}
void TestJsonTag::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestJsonTag* TestJsonTag::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint64_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = TestJsonTag::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}
void TestJsonTag::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestJsonTag::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void TestJsonTag::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestJsonTag::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestJsonTag::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestJsonTagMeta");
}
}
