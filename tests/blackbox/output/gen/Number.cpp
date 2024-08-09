// This file is generated by holgen. Do not modify manually.
#include "Number.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Number::operator==(const Number &rhs) const {
  return !(
      mValue != rhs.mValue
  );
}

int64_t Number::GetValue() const {
  return mValue;
}

void Number::SetValue(int64_t val) {
  mValue = val;
}

bool Number::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("value", name)) {
        auto res = JsonHelper::Parse(mValue, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Number.value field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Number: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mValue, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Number.value field");
  }
  return true;
}

void Number::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "NumberMeta");
  lua_setmetatable(luaState, -2);
}

void Number::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "value");
  LuaHelper::Push(mValue, luaState, true);
  lua_settable(luaState, -3);
}

void Number::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Number *Number::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Number *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Number Number::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Number{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("value", key)) {
      LuaHelper::Read(result.mValue, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Number.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Number::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Number::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("value", key)) {
    auto res = LuaHelper::Read(instance->mValue, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Number.value from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Number.{}", key);
  }
  return 0;
}

void Number::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Number::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Number::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "NumberMeta");
}

int Number::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("value", key)) {
    auto instance = Number::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Number.value with an invalid lua proxy object!");
    LuaHelper::Push(instance->mValue, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Number.{}", key);
    return 0;
  }
  return 1;
}
}
