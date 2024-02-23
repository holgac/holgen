#include "DamageMultiplier.h"

#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::string& DamageMultiplier::GetWhen() const {
  return mWhen;
}
std::string& DamageMultiplier::GetWhen() {
  return mWhen;
}
void DamageMultiplier::SetWhen(const std::string& val) {
  mWhen = val;
}
float DamageMultiplier::GetValue() const {
  return mValue;
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
void DamageMultiplier::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (DamageMultiplier*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
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
    lua_pushstring(ls, "p");
    lua_gettable(ls, -4);
    auto instance = (DamageMultiplier*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("when", key)) {
      LuaHelper::Read(instance->mWhen, ls, -2);
    } else if (0 == strcmp("value", key)) {
      LuaHelper::Read(instance->mValue, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "DamageMultiplierMeta");
}
}
