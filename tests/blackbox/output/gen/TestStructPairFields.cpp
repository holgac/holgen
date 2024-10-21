// This file is generated by holgen. Do not modify manually.
#include "TestStructPairFields.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructPairFields::operator==(const TestStructPairFields &rhs) const {
  return !(
      mIntStringPair != rhs.mIntStringPair ||
      mPairVector != rhs.mPairVector
  );
}

const std::pair<int32_t, std::string> &TestStructPairFields::GetIntStringPair() const {
  return mIntStringPair;
}

std::pair<int32_t, std::string> &TestStructPairFields::GetIntStringPair() {
  return mIntStringPair;
}

const std::vector<std::pair<std::string, int32_t>> &TestStructPairFields::GetPairVector() const {
  return mPairVector;
}

std::vector<std::pair<std::string, int32_t>> &TestStructPairFields::GetPairVector() {
  return mPairVector;
}

void TestStructPairFields::SetIntStringPair(const std::pair<int32_t, std::string> &val) {
  mIntStringPair = val;
}

void TestStructPairFields::SetPairVector(const std::vector<std::pair<std::string, int32_t>> &val) {
  mPairVector = val;
}

bool TestStructPairFields::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("intStringPair", name)) {
        auto res = JsonHelper::Parse(mIntStringPair, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructPairFields.intStringPair field");
      } else if (0 == strcmp("pairVector", name)) {
        auto res = JsonHelper::Parse(mPairVector, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructPairFields.pairVector field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructPairFields: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructPairFields!");
      auto res = JsonHelper::Parse(mIntStringPair, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructPairFields.intStringPair field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructPairFields!");
      auto res = JsonHelper::Parse(mPairVector, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructPairFields.pairVector field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructPairFields!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructPairFields.");
    return false;
  }
  return true;
}

void TestStructPairFields::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructPairFieldsMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructPairFields::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "intStringPair");
  LuaHelper::Push<true>(mIntStringPair, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "pairVector");
  LuaHelper::Push<true>(mPairVector, luaState);
  lua_settable(luaState, -3);
}

void TestStructPairFields::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructPairFields *TestStructPairFields::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructPairFields", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructPairFields *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructPairFields TestStructPairFields::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructPairFields{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("intStringPair", key)) {
      LuaHelper::Read(result.mIntStringPair, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("pairVector", key)) {
      LuaHelper::Read(result.mPairVector, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructPairFields.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructPairFields::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructPairFields::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("intStringPair", key)) {
    auto res = LuaHelper::Read(instance->mIntStringPair, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructPairFields.intStringPair from lua failed!");
  } else if (0 == strcmp("pairVector", key)) {
    auto res = LuaHelper::Read(instance->mPairVector, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructPairFields.pairVector from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructPairFields.{}", key);
  }
  return 0;
}

void TestStructPairFields::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructPairFields::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructPairFields::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructPairFieldsMeta");
}

int TestStructPairFields::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("intStringPair", key)) {
    auto instance = TestStructPairFields::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructPairFields.intStringPair with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mIntStringPair, luaState);
  } else if (0 == strcmp("pairVector", key)) {
    auto instance = TestStructPairFields::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructPairFields.pairVector with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mPairVector, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructPairFields.{}", key);
    return 0;
  }
  return 1;
}
}
