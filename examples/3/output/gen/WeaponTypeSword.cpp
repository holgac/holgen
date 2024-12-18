// This file is generated by holgen. Do not modify manually.
#include "WeaponTypeSword.h"

#include <cmath>
#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
float WeaponTypeSword::GetSharpness() const {
  return mSharpness;
}

bool WeaponTypeSword::GetIsShortSword() const {
  return mIsShortSword;
}

void WeaponTypeSword::SetSharpness(float val) {
  mSharpness = val;
}

void WeaponTypeSword::SetIsShortSword(bool val) {
  mIsShortSword = val;
}

bool WeaponTypeSword::operator==(const WeaponTypeSword &rhs) const {
  return !(
      std::fabs(mSharpness - rhs.mSharpness) >= 0.00001 ||
      mIsShortSword != rhs.mIsShortSword
  );
}

bool WeaponTypeSword::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("sharpness", name)) {
        auto res = JsonHelper::Parse(mSharpness, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeSword.sharpness field");
      } else if (0 == strcmp("isShortSword", name)) {
        auto res = JsonHelper::Parse(mIsShortSword, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeSword.isShortSword field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing WeaponTypeSword: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing WeaponTypeSword!");
      auto res = JsonHelper::Parse(mSharpness, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeSword.sharpness field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing WeaponTypeSword!");
      auto res = JsonHelper::Parse(mIsShortSword, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeSword.isShortSword field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing WeaponTypeSword!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing WeaponTypeSword.");
    return false;
  }
  return true;
}

rapidjson::Value WeaponTypeSword::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("sharpness", JsonHelper::Dump(mSharpness, doc, luaState), doc.GetAllocator());
  val.AddMember("isShortSword", JsonHelper::Dump(mIsShortSword, doc, luaState), doc.GetAllocator());
  return val;
}

void WeaponTypeSword::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponTypeSword");
  lua_setmetatable(luaState, -2);
}

void WeaponTypeSword::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "sharpness");
  LuaHelper::Push<true>(mSharpness, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "isShortSword");
  LuaHelper::Push<true>(mIsShortSword, luaState);
  lua_settable(luaState, -3);
}

void WeaponTypeSword::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

WeaponTypeSword *WeaponTypeSword::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting WeaponTypeSword", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (WeaponTypeSword *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

WeaponTypeSword WeaponTypeSword::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = WeaponTypeSword{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("sharpness", key)) {
      LuaHelper::Read(result.mSharpness, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("isShortSword", key)) {
      LuaHelper::Read(result.mIsShortSword, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: WeaponTypeSword.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int WeaponTypeSword::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponTypeSword::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("sharpness", key)) {
    auto res = LuaHelper::Read(instance->mSharpness, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning WeaponTypeSword.sharpness from lua failed!");
  } else if (0 == strcmp("isShortSword", key)) {
    auto res = LuaHelper::Read(instance->mIsShortSword, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning WeaponTypeSword.isShortSword from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeSword.{}", key);
  }
  return 0;
}

int WeaponTypeSword::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = WeaponTypeSword::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling WeaponTypeSword.operator== method with an invalid lua proxy object!");
  WeaponTypeSword arg0Mirror;
  WeaponTypeSword *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = WeaponTypeSword::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = WeaponTypeSword::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void WeaponTypeSword::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, WeaponTypeSword::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, WeaponTypeSword::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, WeaponTypeSword::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponTypeSword");
}

int WeaponTypeSword::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("sharpness", key)) {
    auto instance = WeaponTypeSword::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponTypeSword.sharpness with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mSharpness, luaState);
  } else if (0 == strcmp("isShortSword", key)) {
    auto instance = WeaponTypeSword::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponTypeSword.isShortSword with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mIsShortSword, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeSword.{}", key);
    return 0;
  }
  return 1;
}
}
