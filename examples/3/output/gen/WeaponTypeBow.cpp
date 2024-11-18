// This file is generated by holgen. Do not modify manually.
#include "WeaponTypeBow.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex3_schemas {
uint32_t WeaponTypeBow::GetRange() const {
  return mRange;
}

const std::string &WeaponTypeBow::GetMaterial() const {
  return mMaterial;
}

std::string &WeaponTypeBow::GetMaterial() {
  return mMaterial;
}

void WeaponTypeBow::SetRange(uint32_t val) {
  mRange = val;
}

void WeaponTypeBow::SetMaterial(const std::string &val) {
  mMaterial = val;
}

bool WeaponTypeBow::operator==(const WeaponTypeBow &rhs) const {
  return !(
      mRange != rhs.mRange ||
      mMaterial != rhs.mMaterial
  );
}

bool WeaponTypeBow::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("range", name)) {
        auto res = JsonHelper::Parse(mRange, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.range field");
      } else if (0 == strcmp("material", name)) {
        auto res = JsonHelper::Parse(mMaterial, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.material field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing WeaponTypeBow: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing WeaponTypeBow!");
      auto res = JsonHelper::Parse(mRange, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.range field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing WeaponTypeBow!");
      auto res = JsonHelper::Parse(mMaterial, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse WeaponTypeBow.material field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing WeaponTypeBow!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing WeaponTypeBow.");
    return false;
  }
  return true;
}

rapidjson::Value WeaponTypeBow::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("range", JsonHelper::Dump(mRange, doc), doc.GetAllocator());
  val.AddMember("material", JsonHelper::Dump(mMaterial, doc), doc.GetAllocator());
  return val;
}

void WeaponTypeBow::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "WeaponTypeBow");
  lua_setmetatable(luaState, -2);
}

void WeaponTypeBow::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "range");
  LuaHelper::Push<true>(mRange, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "material");
  LuaHelper::Push<true>(mMaterial, luaState);
  lua_settable(luaState, -3);
}

void WeaponTypeBow::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

WeaponTypeBow *WeaponTypeBow::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting WeaponTypeBow", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (WeaponTypeBow *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

WeaponTypeBow WeaponTypeBow::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = WeaponTypeBow{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("range", key)) {
      LuaHelper::Read(result.mRange, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("material", key)) {
      LuaHelper::Read(result.mMaterial, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int WeaponTypeBow::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = WeaponTypeBow::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("range", key)) {
    auto res = LuaHelper::Read(instance->mRange, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning WeaponTypeBow.range from lua failed!");
  } else if (0 == strcmp("material", key)) {
    auto res = LuaHelper::Read(instance->mMaterial, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning WeaponTypeBow.material from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
  }
  return 0;
}

void WeaponTypeBow::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, WeaponTypeBow::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, WeaponTypeBow::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "WeaponTypeBow");
}

int WeaponTypeBow::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("range", key)) {
    auto instance = WeaponTypeBow::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponTypeBow.range with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mRange, luaState);
  } else if (0 == strcmp("material", key)) {
    auto instance = WeaponTypeBow::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for WeaponTypeBow.material with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mMaterial, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: WeaponTypeBow.{}", key);
    return 0;
  }
  return 1;
}
}
