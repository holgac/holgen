#include "DamageMultiplier.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::string& DamageMultiplier::GetWhen() const {
  return mWhen;
}
std::string& DamageMultiplier::GetWhen() {
  return mWhen;
}
float DamageMultiplier::GetValue() const {
  return mValue;
}
void DamageMultiplier::SetWhen(const std::string& val) {
  mWhen = val;
}
void DamageMultiplier::SetValue(float val) {
  mValue = val;
}
bool DamageMultiplier::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "when")) {
      auto res = JsonHelper::Parse(mWhen, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "value")) {
      auto res = JsonHelper::Parse(mValue, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void DamageMultiplier::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "DamageMultiplierMeta");
  lua_setmetatable(luaState, -2);
}
void DamageMultiplier::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
DamageMultiplier* DamageMultiplier::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (DamageMultiplier*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void DamageMultiplier::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = DamageMultiplier::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("when", key)) {
      LuaHelper::Push(instance->mWhen, ls);
    } else if (0 == strcmp("value", key)) {
      LuaHelper::Push(instance->mValue, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = DamageMultiplier::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("when", key)) {
      LuaHelper::Read(instance->mWhen, ls, -1);
    } else if (0 == strcmp("value", key)) {
      LuaHelper::Read(instance->mValue, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "DamageMultiplierMeta");
}
}
