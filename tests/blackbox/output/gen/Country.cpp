// This file is generated by holgen. Do not modify manually.
#include "Country.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Country::operator==(const Country &rhs) const {
  return !(
      mLeader != rhs.mLeader ||
      mCitizens != rhs.mCitizens ||
      mPopulation != rhs.mPopulation
  );
}

const Person &Country::GetLeader() const {
  return mLeader;
}

Person &Country::GetLeader() {
  return mLeader;
}

const std::vector<Person> &Country::GetCitizens() const {
  return mCitizens;
}

std::vector<Person> &Country::GetCitizens() {
  return mCitizens;
}

const std::map<uint32_t, uint32_t> &Country::GetPopulation() const {
  return mPopulation;
}

std::map<uint32_t, uint32_t> &Country::GetPopulation() {
  return mPopulation;
}

void Country::SetLeader(const Person &val) {
  mLeader = val;
}

void Country::SetCitizens(const std::vector<Person> &val) {
  mCitizens = val;
}

void Country::SetPopulation(const std::map<uint32_t, uint32_t> &val) {
  mPopulation = val;
}

bool Country::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Country");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("leader", name)) {
      auto res = JsonHelper::Parse(mLeader, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.leader field");
    } else if (0 == strcmp("citizens", name)) {
      auto res = JsonHelper::Parse(mCitizens, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.citizens field");
    } else if (0 == strcmp("population", name)) {
      std::map<std::string, uint32_t> temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.population field");
      mPopulation = std::move(converter.raceU32Map(temp));
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Country: {}", name);
    }
  }
  return true;
}

void Country::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CountryMeta");
  lua_setmetatable(luaState, -2);
}

void Country::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "leader");
  mLeader.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "citizens");
  LuaHelper::Push(mCitizens, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "population");
  LuaHelper::Push(mPopulation, luaState, true);
  lua_settable(luaState, -3);
}

void Country::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Country *Country::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Country *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Country Country::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Country{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("leader", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mLeader = *Person::ReadProxyFromLua(luaState, -1);
      } else {
        result.mLeader = Person::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("citizens", key)) {
      LuaHelper::Read(result.mCitizens, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("population", key)) {
      LuaHelper::Read(result.mPopulation, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Country.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Country::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Country::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("leader", key)) {
    auto res = LuaHelper::Read(instance->mLeader, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Country.leader from lua failed!");
  } else if (0 == strcmp("citizens", key)) {
    auto res = LuaHelper::Read(instance->mCitizens, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Country.citizens from lua failed!");
  } else if (0 == strcmp("population", key)) {
    auto res = LuaHelper::Read(instance->mPopulation, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Country.population from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Country.{}", key);
  }
  return 0;
}

void Country::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Country::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Country::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CountryMeta");
}

int Country::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("leader", key)) {
    auto instance = Country::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Country.leader with an invalid lua proxy object!");
    LuaHelper::Push(instance->mLeader, luaState, false);
  } else if (0 == strcmp("citizens", key)) {
    auto instance = Country::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Country.citizens with an invalid lua proxy object!");
    LuaHelper::Push(instance->mCitizens, luaState, false);
  } else if (0 == strcmp("population", key)) {
    auto instance = Country::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Country.population with an invalid lua proxy object!");
    LuaHelper::Push(instance->mPopulation, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Country.{}", key);
    return 0;
  }
  return 1;
}
}
