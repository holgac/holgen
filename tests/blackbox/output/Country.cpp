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
void Country::SetLeader(const Person& val) {
  mLeader = val;
}
const std::vector<Person>& Country::GetCitizens() const {
  return mCitizens;
}
std::vector<Person>& Country::GetCitizens() {
  return mCitizens;
}
void Country::SetCitizens(const std::vector<Person>& val) {
  mCitizens = val;
}
const std::map<uint32_t, uint32_t>& Country::GetPopulation() const {
  return mPopulation;
}
std::map<uint32_t, uint32_t>& Country::GetPopulation() {
  return mPopulation;
}
void Country::SetPopulation(const std::map<uint32_t, uint32_t>& val) {
  mPopulation = val;
}
bool Country::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "leader")) {
      auto res = mLeader.ParseJson(data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "citizens")) {
      auto res = JsonHelper::Parse(mCitizens, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "population")) {
      std::map<std::string, uint32_t> temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mPopulation = std::move(converter.raceU32Map(temp));
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
void Country::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (Country*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
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
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -4);
    auto instance = (Country*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("leader", key)) {
      LuaHelper::Read(instance->mLeader, ls, -2);
    } else if (0 == strcmp("citizens", key)) {
      LuaHelper::Read(instance->mCitizens, ls, -2);
    } else if (0 == strcmp("population", key)) {
      LuaHelper::Read(instance->mPopulation, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CountryMeta");
}
}
