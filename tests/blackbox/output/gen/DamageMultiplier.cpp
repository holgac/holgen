// This file is generated by holgen. Do not modify manually.
#include "DamageMultiplier.h"

#include <cmath>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool DamageMultiplier::operator==(const DamageMultiplier &rhs) const {
  return !(
      mWhen != rhs.mWhen ||
      std::fabs(mValue - rhs.mValue) >= 0.00001
  );
}

const std::string &DamageMultiplier::GetWhen() const {
  return mWhen;
}

std::string &DamageMultiplier::GetWhen() {
  return mWhen;
}

float DamageMultiplier::GetValue() const {
  return mValue;
}

void DamageMultiplier::SetWhen(const std::string &val) {
  mWhen = val;
}

void DamageMultiplier::SetValue(float val) {
  mValue = val;
}

bool DamageMultiplier::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing DamageMultiplier");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("when", name)) {
      auto res = JsonHelper::Parse(mWhen, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse DamageMultiplier.when field");
    } else if (0 == strcmp("value", name)) {
      auto res = JsonHelper::Parse(mValue, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse DamageMultiplier.value field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing DamageMultiplier: {}", name);
    }
  }
  return true;
}

void DamageMultiplier::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "DamageMultiplierMeta");
  lua_setmetatable(luaState, -2);
}

void DamageMultiplier::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "when");
  LuaHelper::Push(mWhen, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "value");
  LuaHelper::Push(mValue, luaState, true);
  lua_settable(luaState, -3);
}

void DamageMultiplier::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

DamageMultiplier *DamageMultiplier::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (DamageMultiplier *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

DamageMultiplier DamageMultiplier::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = DamageMultiplier{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("when", key)) {
      LuaHelper::Read(result.mWhen, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("value", key)) {
      LuaHelper::Read(result.mValue, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: DamageMultiplier.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int DamageMultiplier::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = DamageMultiplier::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("when", key)) {
    LuaHelper::Read(instance->mWhen, luaState, -1);
  } else if (0 == strcmp("value", key)) {
    LuaHelper::Read(instance->mValue, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: DamageMultiplier.{}", key);
  }
  return 0;
}

void DamageMultiplier::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, DamageMultiplier::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, DamageMultiplier::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "DamageMultiplierMeta");
}

int DamageMultiplier::IndexMetaMethod(lua_State *luaState) {
  auto instance = DamageMultiplier::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for DamageMultiplier.{} with an invalid lua proxy object!", key);
  if (0 == strcmp("when", key)) {
    LuaHelper::Push(instance->mWhen, luaState, false);
  } else if (0 == strcmp("value", key)) {
    LuaHelper::Push(instance->mValue, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: DamageMultiplier.{}", key);
    return 0;
  }
  return 1;
}
}
