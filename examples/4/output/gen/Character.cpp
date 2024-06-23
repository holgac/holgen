// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "DataManager.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Weapon.h"

namespace ex4_schemas {
bool Character::operator==(const Character &rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mPartnerId == rhs.mPartnerId &&
      mWeaponId == rhs.mWeaponId &&
      mArmor == rhs.mArmor;
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

uint32_t Character::GetPartnerId() const {
  return mPartnerId;
}

const Character *Character::GetPartner() const {
  return Character::Get(mPartnerId);
}

Character *Character::GetPartner() {
  return Character::Get(mPartnerId);
}

uint32_t Character::GetWeaponId() const {
  return mWeaponId;
}

const Weapon *Character::GetWeapon() const {
  return Weapon::Get(mWeaponId);
}

Weapon *Character::GetWeapon() {
  return Weapon::Get(mWeaponId);
}

const CharacterArmor &Character::GetArmor() const {
  return mArmor;
}

CharacterArmor &Character::GetArmor() {
  return mArmor;
}

void Character::SetId(uint32_t val) {
  mId = val;
}

void Character::SetName(const std::string &val) {
  mName = val;
}

void Character::SetPartnerId(uint32_t val) {
  mPartnerId = val;
}

void Character::SetWeaponId(uint32_t val) {
  mWeaponId = val;
}

void Character::SetArmor(const CharacterArmor &val) {
  mArmor = val;
}

Character *Character::Get(uint32_t id) {
  return GlobalPointer<DataManager>::GetInstance()->GetCharacter(id);
}

Character *Character::GetFromName(const std::string &key) {
  return GlobalPointer<DataManager>::GetInstance()->GetCharacterFromName(key);
}

bool Character::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Character");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
    } else if (0 == strcmp("partner", name)) {
      auto res = JsonHelper::Parse(mPartnerId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.partner field");
    } else if (0 == strcmp("weapon", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
      mWeaponId = converter.weaponNameToId(temp);
    } else if (0 == strcmp("armor", name)) {
      auto res = JsonHelper::Parse(mArmor, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
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

void Character::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "partner");
  LuaHelper::Push(mPartnerId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "weapon");
  LuaHelper::Push(mWeaponId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armor");
  mArmor.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
}

void Character::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Character *Character::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Character *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Character Character::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Character{};
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
    } else if (0 == strcmp("partnerId", key)) {
      LuaHelper::Read(result.mPartnerId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("weaponId", key)) {
      LuaHelper::Read(result.mWeaponId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armor", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mArmor = *CharacterArmor::ReadProxyFromLua(luaState, -1);
      } else {
        result.mArmor = CharacterArmor::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Character.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Character::IndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("partnerId", key)) {
    LuaHelper::Push(instance->mPartnerId, luaState, false);
  } else if (0 == strcmp("partner", key)) {
    LuaHelper::Push(Character::Get(instance->mPartnerId), luaState, false);
  } else if (0 == strcmp("weaponId", key)) {
    LuaHelper::Push(instance->mWeaponId, luaState, false);
  } else if (0 == strcmp("weapon", key)) {
    LuaHelper::Push(Weapon::Get(instance->mWeaponId), luaState, false);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Push(instance->mArmor, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}

int Character::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("partnerId", key)) {
    LuaHelper::Read(instance->mPartnerId, luaState, -1);
  } else if (0 == strcmp("weaponId", key)) {
    LuaHelper::Read(instance->mWeaponId, luaState, -1);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Read(instance->mArmor, luaState, -1);
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
