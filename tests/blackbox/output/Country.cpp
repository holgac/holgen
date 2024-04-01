#include "Country.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const Person& Country::GetLeader() const {
  return mLeader;
}
Person& Country::GetLeader() {
  return mLeader;
}
const std::vector<Person>& Country::GetCitizens() const {
  return mCitizens;
}
std::vector<Person>& Country::GetCitizens() {
  return mCitizens;
}
const std::map<uint32_t, uint32_t>& Country::GetPopulation() const {
  return mPopulation;
}
std::map<uint32_t, uint32_t>& Country::GetPopulation() {
  return mPopulation;
}
void Country::SetLeader(const Person& val) {
  mLeader = val;
}
void Country::SetCitizens(const std::vector<Person>& val) {
  mCitizens = val;
}
void Country::SetPopulation(const std::map<uint32_t, uint32_t>& val) {
  mPopulation = val;
}
bool Country::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Country");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "leader")) {
      auto res = mLeader.ParseJson(data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Country.leader field");
    } else if (0 == strcmp(name, "citizens")) {
      auto res = JsonHelper::Parse(mCitizens, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Country.citizens field");
    } else if (0 == strcmp(name, "population")) {
      std::map<std::string, uint32_t> temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Country.population field");
      mPopulation = std::move(converter.raceU32Map(temp));
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Country: {}", name);
    }
  }
  return true;
}
void Country::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CountryMeta");
  lua_setmetatable(luaState, -2);
}
void Country::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
Country* Country::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Country*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void Country::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Country::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("leader", key)) {
      LuaHelper::Push(instance->mLeader, ls);
    } else if (0 == strcmp("citizens", key)) {
      LuaHelper::Push(instance->mCitizens, ls);
    } else if (0 == strcmp("population", key)) {
      LuaHelper::Push(instance->mPopulation, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void Country::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Country::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("leader", key)) {
      LuaHelper::Read(instance->mLeader, ls, -1);
    } else if (0 == strcmp("citizens", key)) {
      LuaHelper::Read(instance->mCitizens, ls, -1);
    } else if (0 == strcmp("population", key)) {
      LuaHelper::Read(instance->mPopulation, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void Country::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "CountryMeta");
}
}
