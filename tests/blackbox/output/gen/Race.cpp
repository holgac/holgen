// This file is generated by holgen. Do not modify manually.
#include "Race.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Race::operator==(const Race &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mHairColors != rhs.mHairColors ||
      mNames != rhs.mNames
  );
}

uint32_t Race::GetId() const {
  return mId;
}

const std::string &Race::GetName() const {
  return mName;
}

std::string &Race::GetName() {
  return mName;
}

const std::vector<std::string> &Race::GetHairColors() const {
  return mHairColors;
}

std::vector<std::string> &Race::GetHairColors() {
  return mHairColors;
}

const std::map<std::string, std::vector<std::string>> &Race::GetNames() const {
  return mNames;
}

std::map<std::string, std::vector<std::string>> &Race::GetNames() {
  return mNames;
}

void Race::SetId(uint32_t val) {
  mId = val;
}

void Race::SetName(const std::string &val) {
  mName = val;
}

void Race::SetHairColors(const std::vector<std::string> &val) {
  mHairColors = val;
}

void Race::SetNames(const std::map<std::string, std::vector<std::string>> &val) {
  mNames = val;
}

bool Race::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.name field");
      } else if (0 == strcmp("hairColors", name)) {
        auto res = JsonHelper::Parse(mHairColors, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.hairColors field");
      } else if (0 == strcmp("names", name)) {
        auto res = JsonHelper::Parse(mNames, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.names field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Race: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Race!");
      auto res = JsonHelper::Parse(mId, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Race!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Race!");
      auto res = JsonHelper::Parse(mHairColors, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.hairColors field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Race!");
      auto res = JsonHelper::Parse(mNames, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Race.names field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Race!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Race.");
    return false;
  }
  return true;
}

void Race::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "RaceMeta");
  lua_setmetatable(luaState, -2);
}

void Race::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "hairColors");
  LuaHelper::Push<true>(mHairColors, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "names");
  LuaHelper::Push<true>(mNames, luaState);
  lua_settable(luaState, -3);
}

void Race::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Race *Race::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Race", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Race *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Race Race::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Race{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("hairColors", key)) {
      LuaHelper::Read(result.mHairColors, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("names", key)) {
      LuaHelper::Read(result.mNames, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Race.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Race::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Race::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Race.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Race.name from lua failed!");
  } else if (0 == strcmp("hairColors", key)) {
    auto res = LuaHelper::Read(instance->mHairColors, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Race.hairColors from lua failed!");
  } else if (0 == strcmp("names", key)) {
    auto res = LuaHelper::Read(instance->mNames, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Race.names from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Race.{}", key);
  }
  return 0;
}

void Race::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Race::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Race::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "RaceMeta");
}

int Race::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Race::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Race.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = Race::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Race.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("hairColors", key)) {
    auto instance = Race::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Race.hairColors with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mHairColors, luaState);
  } else if (0 == strcmp("names", key)) {
    auto instance = Race::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Race.names with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mNames, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Race.{}", key);
    return 0;
  }
  return 1;
}
}
