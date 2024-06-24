// This file is generated by holgen. Do not modify manually.
#include "TestStructArray.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestStructArrayCustomData1.h"
#include "TestStructArrayCustomData2.h"

namespace holgen_blackbox_test {
bool TestStructArray::operator==(const TestStructArray &rhs) const {
  return !(
      mType != rhs.mType ||
      mCustomData != rhs.mCustomData
  );
}

const TestStructArrayType &TestStructArray::GetType() const {
  return mType;
}

TestStructArrayType &TestStructArray::GetType() {
  return mType;
}

const std::array<uint8_t, 16> &TestStructArray::GetCustomData() const {
  return mCustomData;
}

std::array<uint8_t, 16> &TestStructArray::GetCustomData() {
  return mCustomData;
}

void TestStructArray::SetType(const TestStructArrayType &val) {
  mType = val;
}

void TestStructArray::SetCustomData(const std::array<uint8_t, 16> &val) {
  mCustomData = val;
}

bool TestStructArray::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestStructArray");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("type", name)) {
      auto res = JsonHelper::Parse(mType, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArray.type field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestStructArray: {}", name);
    }
  }
  return true;
}

void TestStructArray::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructArrayMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructArray::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "type");
  LuaHelper::Push(mType, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructArray::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructArray *TestStructArray::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructArray *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructArray TestStructArray::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructArray{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("type", key)) {
      LuaHelper::Read(result.mType, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructArray::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructArray::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    LuaHelper::Push(instance->mType, luaState, false);
  } else if (0 == strcmp("GetData1", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestStructArray::ReadProxyFromLua(lsInner, -1);
      auto result = instance->GetData1();
      result->PushToLua(lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetData2", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestStructArray::ReadProxyFromLua(lsInner, -1);
      auto result = instance->GetData2();
      result->PushToLua(lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
    return 0;
  }
  return 1;
}

int TestStructArray::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructArray::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    LuaHelper::Read(instance->mType, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
  }
  return 0;
}

void TestStructArray::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructArray::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructArray::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructArrayMeta");
}
}
