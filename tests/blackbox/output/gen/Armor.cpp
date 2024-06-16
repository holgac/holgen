// This file is generated by holgen. Do not modify manually.
#include "Armor.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Armor::operator==(const Armor &rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mAlternativeName == rhs.mAlternativeName &&
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

const std::string &Armor::GetAlternativeName() const {
  return mAlternativeName;
}

std::string &Armor::GetAlternativeName() {
  return mAlternativeName;
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

void Armor::SetAlternativeName(const std::string &val) {
  mAlternativeName = val;
}

void Armor::SetArmorClass(int8_t val) {
  mArmorClass = val;
}

Armor *Armor::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetArmor(id);
}

Armor *Armor::GetFromName(const std::string &key) {
  return GlobalPointer<GameData>::GetInstance()->GetArmorFromName(key);
}

Armor *Armor::GetFromAlternativeName(const std::string &key) {
  return GlobalPointer<GameData>::GetInstance()->GetArmorFromAlternativeName(key);
}

bool Armor::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Armor");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("id", name)) {
      auto res = JsonHelper::Parse(mId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.id field");
    } else if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.name field");
    } else if (0 == strcmp("alternativeName", name)) {
      auto res = JsonHelper::Parse(mAlternativeName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.alternativeName field");
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
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "ArmorMeta");
  lua_setmetatable(luaState, -2);
}

void Armor::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "alternativeName");
  LuaHelper::Push(mAlternativeName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armorClass");
  LuaHelper::Push(mArmorClass, luaState);
  lua_settable(luaState, -3);
}

void Armor::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Armor *Armor::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Armor::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

Armor Armor::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Armor{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
    } else if (0 == strcmp("alternativeName", key)) {
      LuaHelper::Read(result.mAlternativeName, luaState, -1);
    } else if (0 == strcmp("armorClass", key)) {
      LuaHelper::Read(result.mArmorClass, luaState, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int Armor::IndexMetaMethod(lua_State *luaState) {
  auto instance = Armor::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else if (0 == strcmp("alternativeName", key)) {
    LuaHelper::Push(instance->mAlternativeName, luaState);
  } else if (0 == strcmp("armorClass", key)) {
    LuaHelper::Push(instance->mArmorClass, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
    return 0;
  }
  return 1;
}

int Armor::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Armor::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("alternativeName", key)) {
    LuaHelper::Read(instance->mAlternativeName, luaState, -1);
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
