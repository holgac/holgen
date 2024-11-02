// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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

bool Person::operator==(const Person &rhs) const {
  return !(
      mRace != rhs.mRace ||
      mCurrentCountry != rhs.mCurrentCountry ||
      mCurrentCity != rhs.mCurrentCity ||
      mHomeCountry != rhs.mHomeCountry ||
      mPlaceOfBirth != rhs.mPlaceOfBirth
  );
}

bool Person::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
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
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.race field");
      mRace = converter.raceNameToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCountry field");
      mCurrentCountry = converter.countryNameToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCity field");
      mCurrentCity = converter.cityNameToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.homeCountry field");
      mHomeCountry = converter.countryNameToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.placeOfBirth field");
      mPlaceOfBirth = converter.cityNameToId(temp);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Person!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Person.");
    return false;
  }
  return true;
}

void Person::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "PersonMeta");
  lua_setmetatable(luaState, -2);
}

void Person::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "race");
  LuaHelper::Push<true>(mRace, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "currentCountry");
  LuaHelper::Push<true>(mCurrentCountry, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "currentCity");
  LuaHelper::Push<true>(mCurrentCity, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "homeCountry");
  LuaHelper::Push<true>(mHomeCountry, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "placeOfBirth");
  LuaHelper::Push<true>(mPlaceOfBirth, luaState);
  lua_settable(luaState, -3);
}

void Person::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Person *Person::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Person", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Person *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Person Person::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Person{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("race", key)) {
      LuaHelper::Read(result.mRace, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("currentCountry", key)) {
      LuaHelper::Read(result.mCurrentCountry, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("currentCity", key)) {
      LuaHelper::Read(result.mCurrentCity, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("homeCountry", key)) {
      LuaHelper::Read(result.mHomeCountry, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("placeOfBirth", key)) {
      LuaHelper::Read(result.mPlaceOfBirth, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Person.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Person::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Person::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("race", key)) {
    auto res = LuaHelper::Read(instance->mRace, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Person.race from lua failed!");
  } else if (0 == strcmp("currentCountry", key)) {
    auto res = LuaHelper::Read(instance->mCurrentCountry, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Person.currentCountry from lua failed!");
  } else if (0 == strcmp("currentCity", key)) {
    auto res = LuaHelper::Read(instance->mCurrentCity, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Person.currentCity from lua failed!");
  } else if (0 == strcmp("homeCountry", key)) {
    auto res = LuaHelper::Read(instance->mHomeCountry, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Person.homeCountry from lua failed!");
  } else if (0 == strcmp("placeOfBirth", key)) {
    auto res = LuaHelper::Read(instance->mPlaceOfBirth, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Person.placeOfBirth from lua failed!");
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

int Person::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("race", key)) {
    auto instance = Person::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Person.race with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mRace, luaState);
  } else if (0 == strcmp("currentCountry", key)) {
    auto instance = Person::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Person.currentCountry with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mCurrentCountry, luaState);
  } else if (0 == strcmp("currentCity", key)) {
    auto instance = Person::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Person.currentCity with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mCurrentCity, luaState);
  } else if (0 == strcmp("homeCountry", key)) {
    auto instance = Person::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Person.homeCountry with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mHomeCountry, luaState);
  } else if (0 == strcmp("placeOfBirth", key)) {
    auto instance = Person::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Person.placeOfBirth with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mPlaceOfBirth, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Person.{}", key);
    return 0;
  }
  return 1;
}
}
