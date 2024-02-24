#include "Boot.h"

#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Boot::GetId() const {
  return mId;
}
void Boot::SetId(uint32_t val) {
  mId = val;
}
const std::string& Boot::GetName() const {
  return mName;
}
std::string& Boot::GetName() {
  return mName;
}
void Boot::SetName(const std::string& val) {
  mName = val;
}
const std::string& Boot::GetColor() const {
  return mColor;
}
std::string& Boot::GetColor() {
  return mColor;
}
void Boot::SetColor(const std::string& val) {
  mColor = val;
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
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "BootMeta");
  lua_setmetatable(luaState, -2);
}
void Boot::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (Boot*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
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
    lua_pushstring(ls, "p");
    lua_gettable(ls, -4);
    auto instance = (Boot*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -2);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -2);
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Read(instance->mColor, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "BootMeta");
}
}
