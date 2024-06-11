// This file is generated by holgen. Do not modify manually.
#include "WeaponInInventory.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
WeaponInInventory::WeaponInInventory(WeaponInInventory &&rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == WeaponType::Bow) {
    *GetWeaponAsWeaponTypeBow() = std::move(*rhs.GetWeaponAsWeaponTypeBow());
  } else if (mType == WeaponType::Sword) {
    *GetWeaponAsWeaponTypeSword() = std::move(*rhs.GetWeaponAsWeaponTypeSword());
  }
  rhs.ResetType();
}

WeaponInInventory::WeaponInInventory(const WeaponInInventory &rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == WeaponType::Bow) {
    *GetWeaponAsWeaponTypeBow() = *rhs.GetWeaponAsWeaponTypeBow();
  } else if (mType == WeaponType::Sword) {
    *GetWeaponAsWeaponTypeSword() = *rhs.GetWeaponAsWeaponTypeSword();
  }
}

WeaponInInventory::~WeaponInInventory() {
  ResetType();
}

bool WeaponInInventory::operator==(const WeaponInInventory &rhs) const {
  return
      mType == rhs.mType;
}

const WeaponType &WeaponInInventory::GetType() const {
  return mType;
}

WeaponType &WeaponInInventory::GetType() {
  return mType;
}

void WeaponInInventory::SetType(const WeaponType &val) {
  HOLGEN_FAIL_IF(mType != WeaponType::Invalid, "type field was already initialized (as {}), trying to initialize as {}!,", mType, val);
  mType = val;
  if (val == WeaponType::Bow) {
    new (mWeapon.data()) WeaponTypeBow();
  } else if (val == WeaponType::Sword) {
    new (mWeapon.data()) WeaponTypeSword();
  }
}

const WeaponTypeBow *WeaponInInventory::GetWeaponAsWeaponTypeBow() const {
  HOLGEN_FAIL_IF(mType != WeaponType::Bow, "Attempting to get WeaponInInventory.weapon as WeaponTypeBow while its actual type is {}!", mType);
  return reinterpret_cast<const WeaponTypeBow *>(mWeapon.data());
}

WeaponTypeBow *WeaponInInventory::GetWeaponAsWeaponTypeBow() {
  HOLGEN_FAIL_IF(mType != WeaponType::Bow, "Attempting to get WeaponInInventory.weapon as WeaponTypeBow while its actual type is {}!", mType);
  return reinterpret_cast<WeaponTypeBow *>(mWeapon.data());
}

const WeaponTypeSword *WeaponInInventory::GetWeaponAsWeaponTypeSword() const {
  HOLGEN_FAIL_IF(mType != WeaponType::Sword, "Attempting to get WeaponInInventory.weapon as WeaponTypeSword while its actual type is {}!", mType);
  return reinterpret_cast<const WeaponTypeSword *>(mWeapon.data());
}

WeaponTypeSword *WeaponInInventory::GetWeaponAsWeaponTypeSword() {
  HOLGEN_FAIL_IF(mType != WeaponType::Sword, "Attempting to get WeaponInInventory.weapon as WeaponTypeSword while its actual type is {}!", mType);
  return reinterpret_cast<WeaponTypeSword *>(mWeapon.data());
}

void WeaponInInventory::ResetType() {
  if (mType == WeaponType::Invalid) {
    return;
  }
  if (mType == WeaponType::Bow) {
    GetWeaponAsWeaponTypeBow()->~WeaponTypeBow();
  } else if (mType == WeaponType::Sword) {
    GetWeaponAsWeaponTypeSword()->~WeaponTypeSword();
  }
  mType = WeaponType(WeaponType::Invalid);
}

WeaponInInventory &WeaponInInventory::operator=(WeaponInInventory &&rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == WeaponType::Bow) {
    *GetWeaponAsWeaponTypeBow() = std::move(*rhs.GetWeaponAsWeaponTypeBow());
  } else if (mType == WeaponType::Sword) {
    *GetWeaponAsWeaponTypeSword() = std::move(*rhs.GetWeaponAsWeaponTypeSword());
  }
  rhs.ResetType();
  return *this;
}

WeaponInInventory &WeaponInInventory::operator=(const WeaponInInventory &rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == WeaponType::Bow) {
    *GetWeaponAsWeaponTypeBow() = *rhs.GetWeaponAsWeaponTypeBow();
  } else if (mType == WeaponType::Sword) {
    *GetWeaponAsWeaponTypeSword() = *rhs.GetWeaponAsWeaponTypeSword();
  }
  return *this;
}

bool WeaponInInventory::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("type", name)) {
      WeaponType temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponInInventory.type field");
      SetType(temp);
    }
  }
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing WeaponInInventory");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("type", name)) {
    } else if (0 == strcmp("weapon", name)) {
      bool res;
      if (mType == WeaponType::Bow) {
        res = JsonHelper::Parse(*GetWeaponAsWeaponTypeBow(), data.value, converter);
      } else if (mType == WeaponType::Sword) {
        res = JsonHelper::Parse(*GetWeaponAsWeaponTypeSword(), data.value, converter);
      } else {
        HOLGEN_WARN("Could not json-parse WeaponInInventory.weapon variant field, its type {} is unexpected", mType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponInInventory.weapon variant field of type {}", mType);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing WeaponInInventory: {}", name);
    }
  }
  return true;
}

void WeaponInInventory::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponInInventoryMeta");
  lua_setmetatable(luaState, -2);
}

void WeaponInInventory::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

WeaponInInventory *WeaponInInventory::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (WeaponInInventory *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int WeaponInInventory::IndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponInInventory::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    LuaHelper::Push(instance->mType, luaState);
  } else if (0 == strcmp("weapon", key)) {
    switch (instance->mType.GetValue()) {
    case WeaponType::Bow:
      LuaHelper::Push(instance->GetWeaponAsWeaponTypeBow(), luaState);
      break;
    case WeaponType::Sword:
      LuaHelper::Push(instance->GetWeaponAsWeaponTypeSword(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponInInventory.{}", key);
    return 0;
  }
  return 1;
}

int WeaponInInventory::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponInInventory::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    LuaHelper::Read(instance->mType, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponInInventory.{}", key);
  }
  return 0;
}

void WeaponInInventory::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, WeaponInInventory::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, WeaponInInventory::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponInInventoryMeta");
}
}