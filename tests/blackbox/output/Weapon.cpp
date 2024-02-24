#include "Weapon.h"

#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Weapon::GetDamageMin() const {
  return mDamageMin;
}
void Weapon::SetDamageMin(uint32_t val) {
  mDamageMin = val;
}
uint32_t Weapon::GetDamageMax() const {
  return mDamageMax;
}
void Weapon::SetDamageMax(uint32_t val) {
  mDamageMax = val;
}
const std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() const {
  return mDamageMultipliers;
}
std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() {
  return mDamageMultipliers;
}
void Weapon::SetDamageMultipliers(const std::vector<DamageMultiplier>& val) {
  mDamageMultipliers = val;
}
const std::vector<std::string>& Weapon::GetModifiers() const {
  return mModifiers;
}
std::vector<std::string>& Weapon::GetModifiers() {
  return mModifiers;
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
void Weapon::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (Weapon*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("damageMin", key)) {
      LuaHelper::Push(instance->mDamageMin, ls);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Push(instance->mDamageMax, ls);
    } else if (0 == strcmp("damageMultipliers", key)) {
      LuaHelper::Push(instance->mDamageMultipliers, ls);
    } else if (0 == strcmp("modifiers", key)) {
      LuaHelper::Push(instance->mModifiers, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -4);
    auto instance = (Weapon*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("damageMin", key)) {
      LuaHelper::Read(instance->mDamageMin, ls, -2);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Read(instance->mDamageMax, ls, -2);
    } else if (0 == strcmp("damageMultipliers", key)) {
      LuaHelper::Read(instance->mDamageMultipliers, ls, -2);
    } else if (0 == strcmp("modifiers", key)) {
      LuaHelper::Read(instance->mModifiers, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponMeta");
}
}