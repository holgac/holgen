// This file is generated by holgen. Do not modify manually.
#include "TestBitmapStruct.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestBitmapStruct::operator==(const TestBitmapStruct &rhs) const {
  return
      mBitmapField == rhs.mBitmapField;
}

const TestBitmap &TestBitmapStruct::GetBitmapField() const {
  return mBitmapField;
}

TestBitmap &TestBitmapStruct::GetBitmapField() {
  return mBitmapField;
}

void TestBitmapStruct::SetBitmapField(const TestBitmap &val) {
  mBitmapField = val;
}

bool TestBitmapStruct::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestBitmapStruct");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("bitmapField", name)) {
      auto res = JsonHelper::Parse(mBitmapField, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestBitmapStruct.bitmapField field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestBitmapStruct: {}", name);
    }
  }
  return true;
}

void TestBitmapStruct::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestBitmapStructMeta");
  lua_setmetatable(luaState, -2);
}

void TestBitmapStruct::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestBitmapStruct *TestBitmapStruct::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestBitmapStruct *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestBitmapStruct::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("bitmapField", key)) {
    LuaHelper::Push(instance->mBitmapField, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestBitmapStruct.{}", key);
    return 0;
  }
  return 1;
}

int TestBitmapStruct::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("bitmapField", key)) {
    LuaHelper::Read(instance->mBitmapField, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestBitmapStruct.{}", key);
  }
  return 0;
}

void TestBitmapStruct::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestBitmapStruct::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestBitmapStruct::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestBitmapStructMeta");
}
}