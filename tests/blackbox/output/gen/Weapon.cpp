// This file is generated by holgen. Do not modify manually.
#include "Weapon.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Weapon::GetDamageMin() const {
  return mDamageMin;
}

uint32_t Weapon::GetDamageMax() const {
  return mDamageMax;
}

const std::vector<DamageMultiplier> &Weapon::GetDamageMultipliers() const {
  return mDamageMultipliers;
}

std::vector<DamageMultiplier> &Weapon::GetDamageMultipliers() {
  return mDamageMultipliers;
}

const std::vector<std::string> &Weapon::GetModifiers() const {
  return mModifiers;
}

std::vector<std::string> &Weapon::GetModifiers() {
  return mModifiers;
}

void Weapon::SetDamageMin(uint32_t val) {
  mDamageMin = val;
}

void Weapon::SetDamageMax(uint32_t val) {
  mDamageMax = val;
}

void Weapon::SetDamageMultipliers(const std::vector<DamageMultiplier> &val) {
  mDamageMultipliers = val;
}

void Weapon::SetModifiers(const std::vector<std::string> &val) {
  mModifiers = val;
}

bool Weapon::operator==(const Weapon &rhs) const {
  return !(
      mDamageMin != rhs.mDamageMin ||
      mDamageMax != rhs.mDamageMax ||
      mDamageMultipliers != rhs.mDamageMultipliers ||
      mModifiers != rhs.mModifiers
  );
}

bool Weapon::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
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
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Weapon!");
      auto res = JsonHelper::Parse(mDamageMin, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMin field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Weapon!");
      auto res = JsonHelper::Parse(mDamageMax, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMax field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Weapon!");
      auto res = JsonHelper::Parse(mDamageMultipliers, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMultipliers field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Weapon!");
      auto res = JsonHelper::Parse(mModifiers, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.modifiers field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Weapon!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Weapon.");
    return false;
  }
  return true;
}

rapidjson::Value Weapon::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("damageMin", JsonHelper::Dump(mDamageMin, doc), doc.GetAllocator());
  val.AddMember("damageMax", JsonHelper::Dump(mDamageMax, doc), doc.GetAllocator());
  val.AddMember("damageMultipliers", JsonHelper::Dump(mDamageMultipliers, doc), doc.GetAllocator());
  val.AddMember("modifiers", JsonHelper::Dump(mModifiers, doc), doc.GetAllocator());
  return val;
}

void Weapon::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "Weapon");
  lua_setmetatable(luaState, -2);
}

void Weapon::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "damageMin");
  LuaHelper::Push<true>(mDamageMin, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "damageMax");
  LuaHelper::Push<true>(mDamageMax, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "damageMultipliers");
  LuaHelper::Push<true>(mDamageMultipliers, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "modifiers");
  LuaHelper::Push<true>(mModifiers, luaState);
  lua_settable(luaState, -3);
}

void Weapon::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Weapon *Weapon::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Weapon", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Weapon *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Weapon Weapon::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Weapon{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("damageMin", key)) {
      LuaHelper::Read(result.mDamageMin, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Read(result.mDamageMax, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("damageMultipliers", key)) {
      LuaHelper::Read(result.mDamageMultipliers, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("modifiers", key)) {
      LuaHelper::Read(result.mModifiers, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Weapon::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Weapon::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("damageMin", key)) {
    auto res = LuaHelper::Read(instance->mDamageMin, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Weapon.damageMin from lua failed!");
  } else if (0 == strcmp("damageMax", key)) {
    auto res = LuaHelper::Read(instance->mDamageMax, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Weapon.damageMax from lua failed!");
  } else if (0 == strcmp("damageMultipliers", key)) {
    auto res = LuaHelper::Read(instance->mDamageMultipliers, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Weapon.damageMultipliers from lua failed!");
  } else if (0 == strcmp("modifiers", key)) {
    auto res = LuaHelper::Read(instance->mModifiers, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Weapon.modifiers from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
  }
  return 0;
}

void Weapon::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Weapon::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Weapon::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "Weapon");
}

int Weapon::GetAverageDamageCallerFromLua(lua_State *luaState) {
  auto instance = Weapon::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling Weapon.GetAverageDamage method with an invalid lua proxy object!");
  auto result = instance->GetAverageDamage();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int Weapon::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("damageMin", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.damageMin with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mDamageMin, luaState);
  } else if (0 == strcmp("damageMax", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.damageMax with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mDamageMax, luaState);
  } else if (0 == strcmp("damageMultipliers", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.damageMultipliers with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mDamageMultipliers, luaState);
  } else if (0 == strcmp("modifiers", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.modifiers with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mModifiers, luaState);
  } else if (0 == strcmp("GetAverageDamage", key)) {
    lua_pushcfunction(luaState, Weapon::GetAverageDamageCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
    return 0;
  }
  return 1;
}
}
