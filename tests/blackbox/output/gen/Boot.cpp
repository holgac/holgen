// This file is generated by holgen. Do not modify manually.
#include "Boot.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "GameData.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Converter.h"

namespace holgen_blackbox_test {
bool Boot::operator==(const Boot& rhs) const {
  return
      mId == rhs.mId &&
      mName == rhs.mName &&
      mColor == rhs.mColor;
}

uint32_t Boot::GetId() const {
  return mId;
}

const std::string& Boot::GetName() const {
  return mName;
}

std::string& Boot::GetName() {
  return mName;
}

const std::string& Boot::GetColor() const {
  return mColor;
}

std::string& Boot::GetColor() {
  return mColor;
}

void Boot::SetId(uint32_t val) {
  mId = val;
}

void Boot::SetName(const std::string& val) {
  mName = val;
}

void Boot::SetColor(const std::string& val) {
  mColor = val;
}

Boot* Boot::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetBoot(id);
}

Boot* Boot::GetFromName(const std::string& key) {
  return GlobalPointer<GameData>::GetInstance()->GetBootFromName(key);
}

bool Boot::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Boot");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
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
  return true;
}

void Boot::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "BootMeta");
  lua_setmetatable(luaState, -2);
}

void Boot::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Boot* Boot::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Boot::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

int Boot::IndexMetaMethod(lua_State* luaState) {
  auto instance = Boot::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    LuaHelper::Push(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Push(instance->mName, luaState);
  } else if (0 == strcmp("color", key)) {
    LuaHelper::Push(instance->mColor, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Boot.{}", key);
    return 0;
  }
  return 1;
}

int Boot::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = Boot::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    LuaHelper::Read(instance->mId, luaState, -1);
  } else if (0 == strcmp("name", key)) {
    LuaHelper::Read(instance->mName, luaState, -1);
  } else if (0 == strcmp("color", key)) {
    LuaHelper::Read(instance->mColor, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Boot.{}", key);
  }
  return 0;
}

void Boot::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Boot::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Boot::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "BootMeta");
}
}