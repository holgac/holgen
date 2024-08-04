// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructCat.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestVariantStructCat::operator==(const TestVariantStructCat &rhs) const {
  return !(
      mName != rhs.mName ||
      mColor != rhs.mColor
  );
}

const std::string &TestVariantStructCat::GetName() const {
  return mName;
}

std::string &TestVariantStructCat::GetName() {
  return mName;
}

const std::string &TestVariantStructCat::GetColor() const {
  return mColor;
}

std::string &TestVariantStructCat::GetColor() {
  return mColor;
}

void TestVariantStructCat::SetName(const std::string &val) {
  mName = val;
}

void TestVariantStructCat::SetColor(const std::string &val) {
  mColor = val;
}

bool TestVariantStructCat::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestVariantStructCat");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.name field");
    } else if (0 == strcmp("color", name)) {
      auto res = JsonHelper::Parse(mColor, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.color field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructCat: {}", name);
    }
  }
  return true;
}

void TestVariantStructCat::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructCatMeta");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructCat::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "color");
  LuaHelper::Push(mColor, luaState, true);
  lua_settable(luaState, -3);
}

void TestVariantStructCat::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructCat *TestVariantStructCat::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructCat *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestVariantStructCat TestVariantStructCat::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestVariantStructCat{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Read(result.mColor, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestVariantStructCat::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructCat::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("color", key)) {
    LuaHelper::Read(instance->mColor, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
  }
  return 0;
}

void TestVariantStructCat::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructCat::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructCat::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructCatMeta");
}

int TestVariantStructCat::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructCat::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("color", key)) {
    LuaHelper::Push(instance->mColor, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
    return 0;
  }
  return 1;
}
}
