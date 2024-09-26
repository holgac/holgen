// This file is generated by holgen. Do not modify manually.
#include "Boot.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Boot::operator==(const Boot &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mColor != rhs.mColor
  );
}

uint32_t Boot::GetId() const {
  return mId;
}

const std::string &Boot::GetName() const {
  return mName;
}

std::string &Boot::GetName() {
  return mName;
}

const std::string &Boot::GetColor() const {
  return mColor;
}

std::string &Boot::GetColor() {
  return mColor;
}

void Boot::SetId(uint32_t val) {
  mId = val;
}

void Boot::SetName(const std::string &val) {
  mName = val;
}

void Boot::SetColor(const std::string &val) {
  mColor = val;
}

Boot *Boot::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetBoot(id);
}

Boot *Boot::GetFromName(const std::string &key) {
  return GlobalPointer<GameData>::GetInstance()->GetBootFromName(key);
}

bool Boot::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Boot.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Boot.name field");
      } else if (0 == strcmp("color", name)) {
        auto res = JsonHelper::Parse(mColor, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Boot.color field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Boot: {}", name);
      }
    }
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Boot.");
    return false;
  }
  return true;
}

void Boot::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "BootMeta");
  lua_setmetatable(luaState, -2);
}

void Boot::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "color");
  LuaHelper::Push(mColor, luaState, true);
  lua_settable(luaState, -3);
}

void Boot::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Boot *Boot::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Boot::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

Boot Boot::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Boot{};
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
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Read(result.mColor, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Boot.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Boot::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Boot::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Boot.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Boot.name from lua failed!");
  } else if (0 == strcmp("color", key)) {
    auto res = LuaHelper::Read(instance->mColor, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Boot.color from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Boot.{}", key);
  }
  return 0;
}

void Boot::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Boot::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Boot::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "BootMeta");
}

int Boot::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Boot::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Boot.id with an invalid lua proxy object!");
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = Boot::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Boot.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("color", key)) {
    auto instance = Boot::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Boot.color with an invalid lua proxy object!");
    LuaHelper::Push(instance->mColor, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Boot.{}", key);
    return 0;
  }
  return 1;
}
}
