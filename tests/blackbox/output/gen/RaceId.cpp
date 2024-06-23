// This file is generated by holgen. Do not modify manually.
#include "RaceId.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool RaceId::operator==(const RaceId &rhs) const {
  return
      mId == rhs.mId;
}

uint32_t RaceId::GetId() const {
  return mId;
}

void RaceId::SetId(uint32_t val) {
  mId = val;
}

bool RaceId::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse RaceId.id field");
        mId = converter.raceNameToId(temp);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing RaceId: {}", name);
      }
    }
  } else {
    std::string temp;
    auto res = JsonHelper::Parse(temp, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse RaceId.id field");
    mId = converter.raceNameToId(temp);
  }
  return true;
}

void RaceId::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "RaceIdMeta");
  lua_setmetatable(luaState, -2);
}

void RaceId::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
}

void RaceId::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

RaceId *RaceId::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (RaceId *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

RaceId RaceId::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = RaceId{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: RaceId.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int RaceId::IndexMetaMethod(lua_State *luaState) {
  auto instance = RaceId::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: RaceId.{}", key);
    return 0;
  }
  return 1;
}

int RaceId::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = RaceId::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: RaceId.{}", key);
  }
  return 0;
}

void RaceId::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, RaceId::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, RaceId::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "RaceIdMeta");
}
}
