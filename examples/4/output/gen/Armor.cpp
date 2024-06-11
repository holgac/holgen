// This file is generated by holgen. Do not modify manually.
#include "Armor.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "DataManager.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex4_schemas {
bool Armor::operator==(const Armor &rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mArmorClass == rhs.mArmorClass;
}

uint32_t Armor::GetId() const {
  return mId;
}

const std::string &Armor::GetName() const {
  return mName;
}

std::string &Armor::GetName() {
  return mName;
}

int8_t Armor::GetArmorClass() const {
  return mArmorClass;
}

void Armor::SetId(uint32_t val) {
  mId = val;
}

void Armor::SetName(const std::string &val) {
  mName = val;
}

void Armor::SetArmorClass(int8_t val) {
  mArmorClass = val;
}

Armor *Armor::Get(uint32_t id) {
  return GlobalPointer<DataManager>::GetInstance()->GetArmor(id);
}

Armor *Armor::GetFromName(const std::string &key) {
  return GlobalPointer<DataManager>::GetInstance()->GetArmorFromName(key);
}

bool Armor::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Armor");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.name field");
    } else if (0 == strcmp("armorClass", name)) {
      auto res = JsonHelper::Parse(mArmorClass, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.armorClass field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Armor: {}", name);
    }
  }
  return true;
}

void Armor::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "ArmorMeta");
  lua_setmetatable(luaState, -2);
}

void Armor::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Armor *Armor::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Armor *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int Armor::IndexMetaMethod(lua_State *luaState) {
  auto instance = Armor::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else if (0 == strcmp("armorClass", key)) {
    LuaHelper::Push(instance->mArmorClass, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
    return 0;
  }
  return 1;
}

int Armor::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Armor::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("armorClass", key)) {
    LuaHelper::Read(instance->mArmorClass, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
  }
  return 0;
}

void Armor::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Armor::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Armor::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "ArmorMeta");
}
}