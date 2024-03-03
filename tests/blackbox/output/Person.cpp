#include "Person.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Person::GetRace() const {
  return mRace;
}
void Person::SetRace(uint32_t val) {
  mRace = val;
}
uint32_t Person::GetCurrentCountry() const {
  return mCurrentCountry;
}
void Person::SetCurrentCountry(uint32_t val) {
  mCurrentCountry = val;
}
uint32_t Person::GetCurrentCity() const {
  return mCurrentCity;
}
void Person::SetCurrentCity(uint32_t val) {
  mCurrentCity = val;
}
uint32_t Person::GetHomeCountry() const {
  return mHomeCountry;
}
void Person::SetHomeCountry(uint32_t val) {
  mHomeCountry = val;
}
uint32_t Person::GetPlaceOfBirth() const {
  return mPlaceOfBirth;
}
void Person::SetPlaceOfBirth(uint32_t val) {
  mPlaceOfBirth = val;
}
bool Person::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "race")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mRace = converter.raceNameToId(temp);
    } else if (0 == strcmp(name, "currentCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp(name, "currentCity")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCity = converter.cityNameToId(temp);
    } else if (0 == strcmp(name, "homeCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mHomeCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp(name, "placeOfBirth")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mPlaceOfBirth = converter.cityNameToId(temp);
    }
  }
  return true;
}
void Person::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "PersonMeta");
  lua_setmetatable(luaState, -2);
}
void Person::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (Person*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("race", key)) {
      LuaHelper::Push(instance->mRace, ls);
    } else if (0 == strcmp("currentCountry", key)) {
      LuaHelper::Push(instance->mCurrentCountry, ls);
    } else if (0 == strcmp("currentCity", key)) {
      LuaHelper::Push(instance->mCurrentCity, ls);
    } else if (0 == strcmp("homeCountry", key)) {
      LuaHelper::Push(instance->mHomeCountry, ls);
    } else if (0 == strcmp("placeOfBirth", key)) {
      LuaHelper::Push(instance->mPlaceOfBirth, ls);
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
    auto instance = (Person*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("race", key)) {
      LuaHelper::Read(instance->mRace, ls, -2);
    } else if (0 == strcmp("currentCountry", key)) {
      LuaHelper::Read(instance->mCurrentCountry, ls, -2);
    } else if (0 == strcmp("currentCity", key)) {
      LuaHelper::Read(instance->mCurrentCity, ls, -2);
    } else if (0 == strcmp("homeCountry", key)) {
      LuaHelper::Read(instance->mHomeCountry, ls, -2);
    } else if (0 == strcmp("placeOfBirth", key)) {
      LuaHelper::Read(instance->mPlaceOfBirth, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "PersonMeta");
}
}
