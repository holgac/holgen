// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Armor.h"
#include "Boot.h"
#include "Converter.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Character::operator==(const Character &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mBootId != rhs.mBootId ||
      mArmorId != rhs.mArmorId
  );
}

uint32_t Character::GetId() const {
  return mId;
}

const std::string &Character::GetName() const {
  return mName;
}

std::string &Character::GetName() {
  return mName;
}

uint32_t Character::GetBootId() const {
  return mBootId;
}

const Boot *Character::GetBoot() const {
  return Boot::Get(mBootId);
}

Boot *Character::GetBoot() {
  return Boot::Get(mBootId);
}

uint32_t Character::GetArmorId() const {
  return mArmorId;
}

const Armor *Character::GetArmor() const {
  return Armor::Get(mArmorId);
}

Armor *Character::GetArmor() {
  return Armor::Get(mArmorId);
}

void Character::SetId(uint32_t val) {
  mId = val;
}

void Character::SetName(const std::string &val) {
  mName = val;
}

void Character::SetBootId(uint32_t val) {
  mBootId = val;
}

void Character::SetArmorId(uint32_t val) {
  mArmorId = val;
}

Character *Character::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
}

Character *Character::GetFromName(const std::string &key) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacterFromName(key);
}

bool Character::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
      } else if (0 == strcmp("boot", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.boot field");
        mBootId = converter.bootNameToId(temp);
      } else if (0 == strcmp("armor", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
        mArmorId = converter.armorNameToId(temp);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Character: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mId, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.boot field");
      mBootId = converter.bootNameToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
      mArmorId = converter.armorNameToId(temp);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Character!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Character.");
    return false;
  }
  return true;
}

void Character::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CharacterMeta");
  lua_setmetatable(luaState, -2);
}

void Character::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push(mId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push(mName, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "bootId");
  LuaHelper::Push(mBootId, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armorId");
  LuaHelper::Push(mArmorId, luaState, true);
  lua_settable(luaState, -3);
}

void Character::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Character *Character::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Character", className);
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Character::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

Character Character::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Character{};
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
    } else if (0 == strcmp("bootId", key)) {
      LuaHelper::Read(result.mBootId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armorId", key)) {
      LuaHelper::Read(result.mArmorId, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Character.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Character::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.name from lua failed!");
  } else if (0 == strcmp("bootId", key)) {
    auto res = LuaHelper::Read(instance->mBootId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.boot from lua failed!");
  } else if (0 == strcmp("armorId", key)) {
    auto res = LuaHelper::Read(instance->mArmorId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.armor from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
  }
  return 0;
}

void Character::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Character::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Character::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CharacterMeta");
}

int Character::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.id with an invalid lua proxy object!");
    LuaHelper::Push(instance->mId, luaState, false);
  } else if (0 == strcmp("name", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.name with an invalid lua proxy object!");
    LuaHelper::Push(instance->mName, luaState, false);
  } else if (0 == strcmp("bootId", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.boot with an invalid lua proxy object!");
    LuaHelper::Push(instance->mBootId, luaState, false);
  } else if (0 == strcmp("boot", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push(Boot::Get(instance->mBootId), luaState, false);
  } else if (0 == strcmp("armorId", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.armor with an invalid lua proxy object!");
    LuaHelper::Push(instance->mArmorId, luaState, false);
  } else if (0 == strcmp("armor", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push(Armor::Get(instance->mArmorId), luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}
}
