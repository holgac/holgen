#include "Boot.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "GameData.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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
Boot* Boot::GetFromName(const std::string& val) {
  return GlobalPointer<GameData>::GetInstance()->GetBootFromName(val);
}
bool Boot::ParseJson(const rapidjson::Value& json, const Converter& converter) {
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
    } else if (0 == strcmp(name, "color")) {
      auto res = JsonHelper::Parse(mColor, data.value, converter);
      if (!res)
        return false;
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
Boot* Boot::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Boot::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}
void Boot::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Boot::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Push(instance->mColor, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Boot::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -1);
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Read(instance->mColor, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "BootMeta");
}
}
