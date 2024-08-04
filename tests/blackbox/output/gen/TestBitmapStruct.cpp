// This file is generated by holgen. Do not modify manually.
#include "TestBitmapStruct.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestBitmapStruct::operator==(const TestBitmapStruct &rhs) const {
  return !(
      mBitmapField != rhs.mBitmapField
  );
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

bool TestBitmapStruct::HasBitmapField(int64_t val) const {
  return mBitmapField.Has(TestBitmap(val));
}

void TestBitmapStruct::AddBitmapField(int64_t val) {
  mBitmapField.Add(TestBitmap(val));
}

void TestBitmapStruct::RemoveBitmapField(int64_t val) {
  mBitmapField.Remove(TestBitmap(val));
}

void TestBitmapStruct::ToggleBitmapField(int64_t val) {
  mBitmapField.Toggle(TestBitmap(val));
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

void TestBitmapStruct::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "bitmapField");
  LuaHelper::Push(mBitmapField, luaState, true);
  lua_settable(luaState, -3);
}

void TestBitmapStruct::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestBitmapStruct *TestBitmapStruct::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestBitmapStruct *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestBitmapStruct TestBitmapStruct::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestBitmapStruct{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("bitmapField", key)) {
      LuaHelper::Read(result.mBitmapField, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestBitmapStruct.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestBitmapStruct::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -3);
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

int TestBitmapStruct::HasBitmapFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestBitmapStruct.HasBitmapField method with an invalid lua proxy object!");
  int64_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->HasBitmapField(arg0);
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestBitmapStruct::AddBitmapFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestBitmapStruct.AddBitmapField method with an invalid lua proxy object!");
  int64_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->AddBitmapField(arg0);
  return 0;
}

int TestBitmapStruct::RemoveBitmapFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestBitmapStruct.RemoveBitmapField method with an invalid lua proxy object!");
  int64_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->RemoveBitmapField(arg0);
  return 0;
}

int TestBitmapStruct::ToggleBitmapFieldCallerFromLua(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestBitmapStruct.ToggleBitmapField method with an invalid lua proxy object!");
  int64_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->ToggleBitmapField(arg0);
  return 0;
}

int TestBitmapStruct::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestBitmapStruct::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestBitmapStruct.{} with an invalid lua proxy object!", key);
  if (0 == strcmp("bitmapField", key)) {
    LuaHelper::Push(instance->mBitmapField, luaState, false);
  } else if (0 == strcmp("HasBitmapField", key)) {
    lua_pushcfunction(luaState, TestBitmapStruct::HasBitmapFieldCallerFromLua);
  } else if (0 == strcmp("AddBitmapField", key)) {
    lua_pushcfunction(luaState, TestBitmapStruct::AddBitmapFieldCallerFromLua);
  } else if (0 == strcmp("RemoveBitmapField", key)) {
    lua_pushcfunction(luaState, TestBitmapStruct::RemoveBitmapFieldCallerFromLua);
  } else if (0 == strcmp("ToggleBitmapField", key)) {
    lua_pushcfunction(luaState, TestBitmapStruct::ToggleBitmapFieldCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestBitmapStruct.{}", key);
    return 0;
  }
  return 1;
}
}
