#include "Weapon.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "damageMin")) {
      auto res = JsonHelper::Parse(mDamageMin, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "damageMax")) {
      auto res = JsonHelper::Parse(mDamageMax, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "damageMultipliers")) {
      auto res = JsonHelper::Parse(mDamageMultipliers, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "modifiers")) {
      auto res = JsonHelper::Parse(mModifiers, data.value, converter);
      if (!res)
        return false;
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
void Weapon::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Weapon::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("damageMin", key)) {
      LuaHelper::Push(instance->mDamageMin, ls);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Push(instance->mDamageMax, ls);
    } else if (0 == strcmp("damageMultipliers", key)) {
      LuaHelper::Push(instance->mDamageMultipliers, ls);
    } else if (0 == strcmp("modifiers", key)) {
      LuaHelper::Push(instance->mModifiers, ls);
    } else if (0 == strcmp("GetAverageDamage", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = Weapon::ReadFromLua(lsInner, -1);
        auto result = instance->GetAverageDamage();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void Weapon::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Weapon::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("damageMin", key)) {
      LuaHelper::Read(instance->mDamageMin, ls, -1);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Read(instance->mDamageMax, ls, -1);
    } else if (0 == strcmp("damageMultipliers", key)) {
      LuaHelper::Read(instance->mDamageMultipliers, ls, -1);
    } else if (0 == strcmp("modifiers", key)) {
      LuaHelper::Read(instance->mModifiers, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void Weapon::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "WeaponMeta");
}
}
