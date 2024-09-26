// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex2_schemas {
bool Character::operator==(const Character &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mRace != rhs.mRace ||
      mArmor != rhs.mArmor ||
      mWeapon != rhs.mWeapon
  );
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
  if (json.IsObject()) {
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
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mRace, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.race field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mArmor, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mWeapon, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Character!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Character.");
    return false;
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
  lua_pushstring(luaState, "race");
  LuaHelper::Push(mRace, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armor");
  mArmor.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "weapon");
  mWeapon.PushMirrorToLua(luaState);
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
    } else if (0 == strcmp("race", key)) {
      LuaHelper::Read(result.mRace, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armor", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mArmor = *Armor::ReadProxyFromLua(luaState, -1);
      } else {
        result.mArmor = Armor::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("weapon", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mWeapon = *Weapon::ReadProxyFromLua(luaState, -1);
      } else {
        result.mWeapon = Weapon::ReadMirrorFromLua(luaState, -1);
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

int Character::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.name from lua failed!");
  } else if (0 == strcmp("race", key)) {
    auto res = LuaHelper::Read(instance->mRace, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.race from lua failed!");
  } else if (0 == strcmp("armor", key)) {
    auto res = LuaHelper::Read(instance->mArmor, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.armor from lua failed!");
  } else if (0 == strcmp("weapon", key)) {
    auto res = LuaHelper::Read(instance->mWeapon, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.weapon from lua failed!");
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

int Character::InitializeCallerFromLua(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling Character.Initialize method with an invalid lua proxy object!");
  instance->Initialize();
  return 0;
}

int Character::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.id with an invalid lua proxy object!");
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("race", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.race with an invalid lua proxy object!");
    LuaHelper::Push(instance->mRace, luaState, false);
  } else if (0 == strcmp("armor", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.armor with an invalid lua proxy object!");
    LuaHelper::Push(instance->mArmor, luaState, false);
  } else if (0 == strcmp("weapon", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.weapon with an invalid lua proxy object!");
    LuaHelper::Push(instance->mWeapon, luaState, false);
  } else if (0 == strcmp("Initialize", key)) {
    lua_pushcfunction(luaState, Character::InitializeCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}
}
