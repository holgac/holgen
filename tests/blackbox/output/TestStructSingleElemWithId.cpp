// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElemWithId.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t TestStructSingleElemWithId::GetId() const {
  return mId;
}
const std::string& TestStructSingleElemWithId::GetName() const {
  return mName;
}
std::string& TestStructSingleElemWithId::GetName() {
  return mName;
}
void TestStructSingleElemWithId::SetId(uint32_t val) {
  mId = val;
}
void TestStructSingleElemWithId::SetName(const std::string& val) {
  mName = val;
}
bool TestStructSingleElemWithId::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsObject()) {
    for(const auto& data: json.GetObject()) {
      const auto& name = data.name.GetString();
      if (0 == strcmp(name, "id")) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemWithId.id field");
      } else if (0 == strcmp(name, "name")) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemWithId.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructSingleElemWithId: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mName, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemWithId.name field");
  }
  return true;
}
void TestStructSingleElemWithId::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemWithIdMeta");
  lua_setmetatable(luaState, -2);
}
void TestStructSingleElemWithId::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestStructSingleElemWithId* TestStructSingleElemWithId::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElemWithId*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestStructSingleElemWithId::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStructSingleElemWithId::ReadFromLua(ls, -2);
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
void TestStructSingleElemWithId::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStructSingleElemWithId::ReadFromLua(ls, -3);
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
void TestStructSingleElemWithId::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestStructSingleElemWithIdMeta");
}
}
