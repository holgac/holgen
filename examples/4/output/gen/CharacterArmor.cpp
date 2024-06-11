// This file is generated by holgen. Do not modify manually.
#include "CharacterArmor.h"

#include <cmath>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Armor.h"
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex4_schemas {
bool CharacterArmor::operator==(const CharacterArmor &rhs) const {
  return
      std::fabs(mDirtAmount - rhs.mDirtAmount) < 0.00001 &&
      mArmorId == rhs.mArmorId;
}

float CharacterArmor::GetDirtAmount() const {
  return mDirtAmount;
}

uint32_t CharacterArmor::GetArmorId() const {
  return mArmorId;
}

const Armor *CharacterArmor::GetArmor() const {
  return Armor::Get(mArmorId);
}

Armor *CharacterArmor::GetArmor() {
  return Armor::Get(mArmorId);
}

void CharacterArmor::SetDirtAmount(float val) {
  mDirtAmount = val;
}

void CharacterArmor::SetArmorId(uint32_t val) {
  mArmorId = val;
}

bool CharacterArmor::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing CharacterArmor");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("dirtAmount", name)) {
      auto res = JsonHelper::Parse(mDirtAmount, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse CharacterArmor.dirtAmount field");
    } else if (0 == strcmp("armor", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse CharacterArmor.armor field");
      mArmorId = converter.armorNameToId(temp);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing CharacterArmor: {}", name);
    }
  }
  return true;
}

void CharacterArmor::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CharacterArmorMeta");
  lua_setmetatable(luaState, -2);
}

void CharacterArmor::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

CharacterArmor *CharacterArmor::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (CharacterArmor *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int CharacterArmor::IndexMetaMethod(lua_State *luaState) {
  auto instance = CharacterArmor::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("dirtAmount", key)) {
    LuaHelper::Push(instance->mDirtAmount, luaState);
  } else if (0 == strcmp("armorId", key)) {
    LuaHelper::Push(instance->mArmorId, luaState);
  } else if (0 == strcmp("armor", key)) {
    LuaHelper::Push(Armor::Get(instance->mArmorId), luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: CharacterArmor.{}", key);
    return 0;
  }
  return 1;
}

int CharacterArmor::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = CharacterArmor::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("dirtAmount", key)) {
    LuaHelper::Read(instance->mDirtAmount, luaState, -1);
  } else if (0 == strcmp("armorId", key)) {
    LuaHelper::Read(instance->mArmorId, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: CharacterArmor.{}", key);
  }
  return 0;
}

void CharacterArmor::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, CharacterArmor::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, CharacterArmor::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CharacterArmorMeta");
}
}