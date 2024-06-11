// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex2_schemas {
bool Character::operator==(const Character &rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mRace == rhs.mRace &&
      mArmor == rhs.mArmor &&
      mWeapon == rhs.mWeapon;
}

uint32_t Character::GetId() const {
  return mId;
}

const std::string &Character::GetName() const {
  return mName;
}

std::string &Character::GetName() {
  return mName;
}

const Race &Character::GetRace() const {
  return mRace;
}

Race &Character::GetRace() {
  return mRace;
}

const Armor &Character::GetArmor() const {
  return mArmor;
}

Armor &Character::GetArmor() {
  return mArmor;
}

const Weapon &Character::GetWeapon() const {
  return mWeapon;
}

Weapon &Character::GetWeapon() {
  return mWeapon;
}

void Character::SetId(uint32_t val) {
  mId = val;
}

void Character::SetName(const std::string &val) {
  mName = val;
}

void Character::SetRace(const Race &val) {
  mRace = val;
}

void Character::SetArmor(const Armor &val) {
  mArmor = val;
}

void Character::SetWeapon(const Weapon &val) {
  mWeapon = val;
}

bool Character::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Character");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
    } else if (0 == strcmp("race", name)) {
      auto res = JsonHelper::Parse(mRace, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.race field");
    } else if (0 == strcmp("armor", name)) {
      auto res = JsonHelper::Parse(mArmor, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
    } else if (0 == strcmp("weapon", name)) {
      auto res = JsonHelper::Parse(mWeapon, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Character: {}", name);
    }
  }
  return true;
}

void Character::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CharacterMeta");
  lua_setmetatable(luaState, -2);
}

void Character::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Character *Character::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Character *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int Character::IndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else if (0 == strcmp("race", key)) {
    LuaHelper::Push(instance->mRace, luaState);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Push(instance->mArmor, luaState);
  } else if (0 == strcmp("weapon", key)) {
    LuaHelper::Push(instance->mWeapon, luaState);
  } else if (0 == strcmp("Initialize", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = Character::ReadFromLua(lsInner, -1);
      instance->Initialize();
      return 0;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}

int Character::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("race", key)) {
    LuaHelper::Read(instance->mRace, luaState, -1);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Read(instance->mArmor, luaState, -1);
  } else if (0 == strcmp("weapon", key)) {
    LuaHelper::Read(instance->mWeapon, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
  }
  return 0;
}

void Character::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Character::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Character::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CharacterMeta");
}
}