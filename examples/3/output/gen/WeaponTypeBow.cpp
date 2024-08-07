// This file is generated by holgen. Do not modify manually.
#include "WeaponTypeBow.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
bool WeaponTypeBow::operator==(const WeaponTypeBow &rhs) const {
  return !(
      mRange != rhs.mRange ||
      mMaterial != rhs.mMaterial
  );
}

uint32_t WeaponTypeBow::GetRange() const {
  return mRange;
}

const std::string &WeaponTypeBow::GetMaterial() const {
  return mMaterial;
}

std::string &WeaponTypeBow::GetMaterial() {
  return mMaterial;
}

void WeaponTypeBow::SetRange(uint32_t val) {
  mRange = val;
}

void WeaponTypeBow::SetMaterial(const std::string &val) {
  mMaterial = val;
}

bool WeaponTypeBow::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing WeaponTypeBow");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("range", name)) {
      auto res = JsonHelper::Parse(mRange, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.range field");
    } else if (0 == strcmp("material", name)) {
      auto res = JsonHelper::Parse(mMaterial, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.material field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing WeaponTypeBow: {}", name);
    }
  }
  return true;
}

void WeaponTypeBow::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponTypeBowMeta");
  lua_setmetatable(luaState, -2);
}

void WeaponTypeBow::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "range");
  LuaHelper::Push(mRange, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "material");
  LuaHelper::Push(mMaterial, luaState, true);
  lua_settable(luaState, -3);
}

void WeaponTypeBow::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

WeaponTypeBow *WeaponTypeBow::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (WeaponTypeBow *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

WeaponTypeBow WeaponTypeBow::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = WeaponTypeBow{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("range", key)) {
      LuaHelper::Read(result.mRange, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("material", key)) {
      LuaHelper::Read(result.mMaterial, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int WeaponTypeBow::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponTypeBow::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("range", key)) {
    LuaHelper::Read(instance->mRange, luaState, -1);
  } else if (0 == strcmp("material", key)) {
    LuaHelper::Read(instance->mMaterial, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
  }
  return 0;
}

void WeaponTypeBow::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, WeaponTypeBow::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, WeaponTypeBow::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponTypeBowMeta");
}

int WeaponTypeBow::IndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponTypeBow::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponTypeBow.{} with an invalid lua proxy object!", key);
  if (0 == strcmp("range", key)) {
    LuaHelper::Push(instance->mRange, luaState, false);
  } else if (0 == strcmp("material", key)) {
    LuaHelper::Push(instance->mMaterial, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
    return 0;
  }
  return 1;
}
}
