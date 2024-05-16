// This file is generated by holgen. Do not modify manually.
#include "Weapon.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
bool Weapon::operator==(const Weapon& rhs) const {
  return
      mDamageMin == rhs.mDamageMin &&
      mDamageMax == rhs.mDamageMax &&
      mDamageMultipliers == rhs.mDamageMultipliers &&
      mModifiers == rhs.mModifiers;
}

uint32_t Weapon::GetDamageMin() const {
  return mDamageMin;
}

uint32_t Weapon::GetDamageMax() const {
  return mDamageMax;
}

const std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() const {
  return mDamageMultipliers;
}

std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() {
  return mDamageMultipliers;
}

const std::vector<std::string>& Weapon::GetModifiers() const {
  return mModifiers;
}

std::vector<std::string>& Weapon::GetModifiers() {
  return mModifiers;
}

void Weapon::SetDamageMin(uint32_t val) {
  mDamageMin = val;
}

void Weapon::SetDamageMax(uint32_t val) {
  mDamageMax = val;
}

void Weapon::SetDamageMultipliers(const std::vector<DamageMultiplier>& val) {
  mDamageMultipliers = val;
}

void Weapon::SetModifiers(const std::vector<std::string>& val) {
  mModifiers = val;
}

bool Weapon::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Weapon");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("damageMin", name)) {
      auto res = JsonHelper::Parse(mDamageMin, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMin field");
    } else if (0 == strcmp("damageMax", name)) {
      auto res = JsonHelper::Parse(mDamageMax, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMax field");
    } else if (0 == strcmp("damageMultipliers", name)) {
      auto res = JsonHelper::Parse(mDamageMultipliers, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMultipliers field");
    } else if (0 == strcmp("modifiers", name)) {
      auto res = JsonHelper::Parse(mModifiers, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.modifiers field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Weapon: {}", name);
    }
  }
  return true;
}

void Weapon::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponMeta");
  lua_setmetatable(luaState, -2);
}

void Weapon::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Weapon* Weapon::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Weapon*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int Weapon::IndexMetaMethod(lua_State* luaState) {
  auto instance = Weapon::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("damageMin", key)) {
    LuaHelper::Push(instance->mDamageMin, luaState);
  } else if (0 == strcmp("damageMax", key)) {
    LuaHelper::Push(instance->mDamageMax, luaState);
  } else if (0 == strcmp("damageMultipliers", key)) {
    LuaHelper::Push(instance->mDamageMultipliers, luaState);
  } else if (0 == strcmp("modifiers", key)) {
    LuaHelper::Push(instance->mModifiers, luaState);
  } else if (0 == strcmp("GetAverageDamage", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = Weapon::ReadFromLua(lsInner, -1);
      auto result = instance->GetAverageDamage();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
    return 0;
  }
  return 1;
}

int Weapon::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = Weapon::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("damageMin", key)) {
    LuaHelper::Read(instance->mDamageMin, luaState, -1);
  } else if (0 == strcmp("damageMax", key)) {
    LuaHelper::Read(instance->mDamageMax, luaState, -1);
  } else if (0 == strcmp("damageMultipliers", key)) {
    LuaHelper::Read(instance->mDamageMultipliers, luaState, -1);
  } else if (0 == strcmp("modifiers", key)) {
    LuaHelper::Read(instance->mModifiers, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
  }
  return 0;
}

void Weapon::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Weapon::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Weapon::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponMeta");
}
}
