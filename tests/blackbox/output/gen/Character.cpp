// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "GameData.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Character::operator==(const Character& rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mBootId == rhs.mBootId &&
      mArmorId == rhs.mArmorId;
}

uint32_t Character::GetId() const {
  return mId;
}

const std::string& Character::GetName() const {
  return mName;
}

std::string& Character::GetName() {
  return mName;
}

uint32_t Character::GetBootId() const {
  return mBootId;
}

const Boot* Character::GetBoot() const {
  return Boot::Get(mBootId);
}

Boot* Character::GetBoot() {
  return Boot::Get(mBootId);
}

uint32_t Character::GetArmorId() const {
  return mArmorId;
}

const Armor* Character::GetArmor() const {
  return Armor::Get(mArmorId);
}

Armor* Character::GetArmor() {
  return Armor::Get(mArmorId);
}

void Character::SetId(uint32_t val) {
  mId = val;
}

void Character::SetName(const std::string& val) {
  mName = val;
}

void Character::SetBootId(uint32_t val) {
  mBootId = val;
}

void Character::SetArmorId(uint32_t val) {
  mArmorId = val;
}

Character* Character::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
}

Character* Character::GetFromName(const std::string& key) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacterFromName(key);
}

bool Character::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Character");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("id", name)) {
      auto res = JsonHelper::Parse(mId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.id field");
    } else if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
    } else if (0 == strcmp("boot", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.boot field");
      mBootId = converter.bootNameToId(temp);
    } else if (0 == strcmp("armor", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
      mArmorId = converter.armorNameToId(temp);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Character: {}", name);
    }
  }
  return true;
}

void Character::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CharacterMeta");
  lua_setmetatable(luaState, -2);
}

void Character::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Character* Character::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Character::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

int Character::IndexMetaMethod(lua_State* luaState) {
  auto instance = Character::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else if (0 == strcmp("bootId", key)) {
    LuaHelper::Push(instance->mBootId, luaState);
  } else if (0 == strcmp("boot", key)) {
    LuaHelper::Push(Boot::Get(instance->mBootId), luaState);
  } else if (0 == strcmp("armorId", key)) {
    LuaHelper::Push(instance->mArmorId, luaState);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Push(Armor::Get(instance->mArmorId), luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}

int Character::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = Character::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("bootId", key)) {
    LuaHelper::Read(instance->mBootId, luaState, -1);
  } else if (0 == strcmp("armorId", key)) {
    LuaHelper::Read(instance->mArmorId, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
  }
  return 0;
}

void Character::CreateLuaMetatable(lua_State* luaState) {
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
