// This file is generated by holgen. Do not modify manually.
#include "WeaponInInventory.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
WeaponInInventory::WeaponInInventory(const WeaponInInventory &rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == WeaponType::Bow) {
    *GetWeaponAsWeaponTypeBow() = *rhs.GetWeaponAsWeaponTypeBow();
  } else if (mType == WeaponType::Sword) {
    *GetWeaponAsWeaponTypeSword() = *rhs.GetWeaponAsWeaponTypeSword();
  }
}

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

WeaponInInventory::~WeaponInInventory() {
  ResetType();
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

void WeaponInInventory::SetType(const WeaponType &val) {
  HOLGEN_FAIL_IF(mType != WeaponType::Invalid, "type field was already initialized (as {}), trying to initialize as {}!,", mType, val);
  mType = val;
  if (val == WeaponType::Bow) {
    new (mWeapon.data()) WeaponTypeBow();
  } else if (val == WeaponType::Sword) {
    new (mWeapon.data()) WeaponTypeSword();
  }
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

WeaponType WeaponInInventory::GetType() const {
  return mType;
}

bool WeaponInInventory::operator==(const WeaponInInventory &rhs) const {
  if (
      mType != rhs.mType
  ) {
    return false;
  }
  if (mType == WeaponType::Bow) {
    if (!(*GetWeaponAsWeaponTypeBow() == *rhs.GetWeaponAsWeaponTypeBow())) {
      return false;
    }
  } else if (mType == WeaponType::Sword) {
    if (!(*GetWeaponAsWeaponTypeSword() == *rhs.GetWeaponAsWeaponTypeSword())) {
      return false;
    }
  }
  return true;
}

bool WeaponInInventory::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("type", name)) {
        WeaponType temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponInInventory.type field");
        SetType(temp);
      }
    }
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
  } else {
    HOLGEN_WARN("Unexpected json type when parsing WeaponInInventory.");
    return false;
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

void WeaponInInventory::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "type");
  LuaHelper::Push(mType, luaState, true);
  lua_settable(luaState, -3);
}

void WeaponInInventory::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

WeaponInInventory *WeaponInInventory::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (WeaponInInventory *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

WeaponInInventory WeaponInInventory::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = WeaponInInventory{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("type", key)) {
      LuaHelper::Read(result.mType, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: WeaponInInventory.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int WeaponInInventory::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponInInventory::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    auto res = LuaHelper::Read(instance->mType, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning WeaponInInventory.type from lua failed!");
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

int WeaponInInventory::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    auto instance = WeaponInInventory::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponInInventory.type with an invalid lua proxy object!");
    LuaHelper::Push(instance->mType, luaState, false);
  } else if (0 == strcmp("weapon", key)) {
    auto instance = WeaponInInventory::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponInInventory.weapon with an invalid lua proxy object!");
    switch (instance->mType.GetValue()) {
    case WeaponType::Bow:
      LuaHelper::Push(instance->GetWeaponAsWeaponTypeBow(), luaState, false);
      break;
    case WeaponType::Sword:
      LuaHelper::Push(instance->GetWeaponAsWeaponTypeSword(), luaState, false);
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
}
