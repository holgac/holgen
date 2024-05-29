// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
bool Person::operator==(const Person &rhs) const {
  return
      mRace == rhs.mRace &&
      mCurrentCountry == rhs.mCurrentCountry &&
      mCurrentCity == rhs.mCurrentCity &&
      mHomeCountry == rhs.mHomeCountry &&
      mPlaceOfBirth == rhs.mPlaceOfBirth;
}

uint32_t Person::GetRace() const {
  return mRace;
}

uint32_t Person::GetCurrentCountry() const {
  return mCurrentCountry;
}

uint32_t Person::GetCurrentCity() const {
  return mCurrentCity;
}

uint32_t Person::GetHomeCountry() const {
  return mHomeCountry;
}

uint32_t Person::GetPlaceOfBirth() const {
  return mPlaceOfBirth;
}

void Person::SetRace(uint32_t val) {
  mRace = val;
}

void Person::SetCurrentCountry(uint32_t val) {
  mCurrentCountry = val;
}

void Person::SetCurrentCity(uint32_t val) {
  mCurrentCity = val;
}

void Person::SetHomeCountry(uint32_t val) {
  mHomeCountry = val;
}

void Person::SetPlaceOfBirth(uint32_t val) {
  mPlaceOfBirth = val;
}

bool Person::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Person");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("race", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.race field");
      mRace = converter.raceNameToId(temp);
    } else if (0 == strcmp("currentCountry", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCountry field");
      mCurrentCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp("currentCity", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCity field");
      mCurrentCity = converter.cityNameToId(temp);
    } else if (0 == strcmp("homeCountry", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.homeCountry field");
      mHomeCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp("placeOfBirth", name)) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.placeOfBirth field");
      mPlaceOfBirth = converter.cityNameToId(temp);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Person: {}", name);
    }
  }
  return true;
}

void Person::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "PersonMeta");
  lua_setmetatable(luaState, -2);
}

void Person::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Person *Person::ReadFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Person *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int Person::IndexMetaMethod(lua_State *luaState) {
  auto instance = Person::ReadFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("race", key)) {
    LuaHelper::Push(instance->mRace, luaState);
  } else if (0 == strcmp("currentCountry", key)) {
    LuaHelper::Push(instance->mCurrentCountry, luaState);
  } else if (0 == strcmp("currentCity", key)) {
    LuaHelper::Push(instance->mCurrentCity, luaState);
  } else if (0 == strcmp("homeCountry", key)) {
    LuaHelper::Push(instance->mHomeCountry, luaState);
  } else if (0 == strcmp("placeOfBirth", key)) {
    LuaHelper::Push(instance->mPlaceOfBirth, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Person.{}", key);
    return 0;
  }
  return 1;
}

int Person::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Person::ReadFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("race", key)) {
    LuaHelper::Read(instance->mRace, luaState, -1);
  } else if (0 == strcmp("currentCountry", key)) {
    LuaHelper::Read(instance->mCurrentCountry, luaState, -1);
  } else if (0 == strcmp("currentCity", key)) {
    LuaHelper::Read(instance->mCurrentCity, luaState, -1);
  } else if (0 == strcmp("homeCountry", key)) {
    LuaHelper::Read(instance->mHomeCountry, luaState, -1);
  } else if (0 == strcmp("placeOfBirth", key)) {
    LuaHelper::Read(instance->mPlaceOfBirth, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Person.{}", key);
  }
  return 0;
}

void Person::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Person::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Person::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "PersonMeta");
}
}
