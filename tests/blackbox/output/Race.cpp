#include "Race.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Race::GetId() const {
  return mId;
}
const std::string& Race::GetName() const {
  return mName;
}
std::string& Race::GetName() {
  return mName;
}
const std::vector<std::string>& Race::GetHairColors() const {
  return mHairColors;
}
std::vector<std::string>& Race::GetHairColors() {
  return mHairColors;
}
const std::map<std::string, std::vector<std::string>>& Race::GetNames() const {
  return mNames;
}
std::map<std::string, std::vector<std::string>>& Race::GetNames() {
  return mNames;
}
void Race::SetId(uint32_t val) {
  mId = val;
}
void Race::SetName(const std::string& val) {
  mName = val;
}
void Race::SetHairColors(const std::vector<std::string>& val) {
  mHairColors = val;
}
void Race::SetNames(const std::map<std::string, std::vector<std::string>>& val) {
  mNames = val;
}
bool Race::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "id")) {
      auto res = JsonHelper::Parse(mId, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "name")) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "hairColors")) {
      auto res = JsonHelper::Parse(mHairColors, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "names")) {
      auto res = JsonHelper::Parse(mNames, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void Race::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "RaceMeta");
  lua_setmetatable(luaState, -2);
}
Race* Race::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Race*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void Race::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Race::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("hairColors", key)) {
      LuaHelper::Push(instance->mHairColors, ls);
    } else if (0 == strcmp("names", key)) {
      LuaHelper::Push(instance->mNames, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Race::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -1);
    } else if (0 == strcmp("hairColors", key)) {
      LuaHelper::Read(instance->mHairColors, ls, -1);
    } else if (0 == strcmp("names", key)) {
      LuaHelper::Read(instance->mNames, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "RaceMeta");
}
}
