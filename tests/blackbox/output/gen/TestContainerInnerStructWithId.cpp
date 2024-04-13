// This file is generated by holgen. Do not modify manually.
#include "TestContainerInnerStructWithId.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerInnerStructWithId::operator==(const TestContainerInnerStructWithId& rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName;
}

uint32_t TestContainerInnerStructWithId::GetId() const {
  return mId;
}

const std::string& TestContainerInnerStructWithId::GetName() const {
  return mName;
}

std::string& TestContainerInnerStructWithId::GetName() {
  return mName;
}

void TestContainerInnerStructWithId::SetId(uint32_t val) {
  mId = val;
}

void TestContainerInnerStructWithId::SetName(const std::string& val) {
  mName = val;
}

bool TestContainerInnerStructWithId::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsObject()) {
    for(const auto& data: json.GetObject()) {
      const auto& name = data.name.GetString();
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
  } else {
    auto res = JsonHelper::Parse(mName, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerInnerStructWithId.name field");
  }
  return true;
}

void TestContainerInnerStructWithId::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerInnerStructWithIdMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerInnerStructWithId::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerInnerStructWithId* TestContainerInnerStructWithId::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerInnerStructWithId*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestContainerInnerStructWithId::IndexMetaMethod(lua_State* luaState) {
  auto instance = TestContainerInnerStructWithId::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructWithId.{}", key);
    return 0;
  }
  return 1;
}

int TestContainerInnerStructWithId::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = TestContainerInnerStructWithId::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerInnerStructWithId.{}", key);
  }
  return 0;
}

void TestContainerInnerStructWithId::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerInnerStructWithId::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerInnerStructWithId::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerInnerStructWithIdMeta");
}
}
