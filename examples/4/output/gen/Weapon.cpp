// This file is generated by holgen. Do not modify manually.
#include "Weapon.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "DataManager.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex4_schemas {
bool Weapon::operator==(const Weapon &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mDamageMin != rhs.mDamageMin ||
      mDamageMax != rhs.mDamageMax
  );
}

uint32_t Weapon::GetId() const {
  return mId;
}

const std::string &Weapon::GetName() const {
  return mName;
}

std::string &Weapon::GetName() {
  return mName;
}

uint8_t Weapon::GetDamageMin() const {
  return mDamageMin;
}

uint8_t Weapon::GetDamageMax() const {
  return mDamageMax;
}

void Weapon::SetId(uint32_t val) {
  mId = val;
}

void Weapon::SetName(const std::string &val) {
  mName = val;
}

void Weapon::SetDamageMin(uint8_t val) {
  mDamageMin = val;
}

void Weapon::SetDamageMax(uint8_t val) {
  mDamageMax = val;
}

Weapon *Weapon::Get(uint32_t id) {
  return GlobalPointer<DataManager>::GetInstance()->GetWeapon(id);
}

Weapon *Weapon::GetFromName(const std::string &key) {
  return GlobalPointer<DataManager>::GetInstance()->GetWeaponFromName(key);
}

bool Weapon::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Weapon");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.name field");
    } else if (0 == strcmp("damageMin", name)) {
      auto res = JsonHelper::Parse(mDamageMin, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMin field");
    } else if (0 == strcmp("damageMax", name)) {
      auto res = JsonHelper::Parse(mDamageMax, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Weapon.damageMax field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Weapon: {}", name);
    }
  }
  return true;
}

void Weapon::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponMeta");
  lua_setmetatable(luaState, -2);
}

void Weapon::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "damageMin");
  LuaHelper::Push(mDamageMin, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "damageMax");
  LuaHelper::Push(mDamageMax, luaState, true);
  lua_settable(luaState, -3);
}

void Weapon::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Weapon *Weapon::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
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
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("damageMin", key)) {
      LuaHelper::Read(result.mDamageMin, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("damageMax", key)) {
      LuaHelper::Read(result.mDamageMax, luaState, -1);
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
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("damageMin", key)) {
    LuaHelper::Read(instance->mDamageMin, luaState, -1);
  } else if (0 == strcmp("damageMax", key)) {
    LuaHelper::Read(instance->mDamageMax, luaState, -1);
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
  lua_setglobal(luaState, "WeaponMeta");
}

int Weapon::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.id with an invalid lua proxy object!");
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("damageMin", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.damageMin with an invalid lua proxy object!");
    LuaHelper::Push(instance->mDamageMin, luaState, false);
  } else if (0 == strcmp("damageMax", key)) {
    auto instance = Weapon::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Weapon.damageMax with an invalid lua proxy object!");
    LuaHelper::Push(instance->mDamageMax, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Weapon.{}", key);
    return 0;
  }
  return 1;
}
}
