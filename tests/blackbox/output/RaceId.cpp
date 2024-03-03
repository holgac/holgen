#include "RaceId.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t RaceId::GetId() const {
  return mId;
}
void RaceId::SetId(uint32_t val) {
  mId = val;
}
bool RaceId::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "id")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mId = converter.raceNameToId(temp);
    }
  }
  return true;
}
void RaceId::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "RaceIdMeta");
  lua_setmetatable(luaState, -2);
}
void RaceId::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (RaceId*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
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
    auto instance = (RaceId*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "RaceIdMeta");
}
}
