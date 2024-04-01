#include "Character.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "GameData.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Character::GetId() const {
  return mId;
}
const std::string& Character::GetName() const {
  return mName;
}
std::string& Character::GetName() {
  return mName;
}
uint32_t Character::GetBootId() const {
  return mBootId;
}
const Boot* Character::GetBoot() const {
  return Boot::Get(mBootId);
}
Boot* Character::GetBoot() {
  return Boot::Get(mBootId);
}
uint32_t Character::GetArmorId() const {
  return mArmorId;
}
const Armor* Character::GetArmor() const {
  return Armor::Get(mArmorId);
}
Armor* Character::GetArmor() {
  return Armor::Get(mArmorId);
}
void Character::SetId(uint32_t val) {
  mId = val;
}
void Character::SetName(const std::string& val) {
  mName = val;
}
void Character::SetBootId(uint32_t val) {
  mBootId = val;
}
void Character::SetArmorId(uint32_t val) {
  mArmorId = val;
}
Character* Character::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
}
Character* Character::GetFromName(const std::string& key) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacterFromName(key);
}
bool Character::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Character");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "id")) {
      auto res = JsonHelper::Parse(mId, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Character.id field");
    } else if (0 == strcmp(name, "name")) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Character.name field");
    } else if (0 == strcmp(name, "boot")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Character.boot field");
      mBootId = converter.bootNameToId(temp);
    } else if (0 == strcmp(name, "armor")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Could not json-parse Character.armor field");
      mArmorId = converter.armorNameToId(temp);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Character: {}", name);
    }
  }
  return true;
}
void Character::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CharacterMeta");
  lua_setmetatable(luaState, -2);
}
void Character::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
Character* Character::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Character::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}
void Character::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Character::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("bootId", key)) {
      LuaHelper::Push(instance->mBootId, ls);
    } else if (0 == strcmp("boot", key)) {
      LuaHelper::Push(Boot::Get(instance->mBootId), ls);
    } else if (0 == strcmp("armorId", key)) {
      LuaHelper::Push(instance->mArmorId, ls);
    } else if (0 == strcmp("armor", key)) {
      LuaHelper::Push(Armor::Get(instance->mArmorId), ls);
    } else if (0 == strcmp("GetBoot", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = Character::ReadFromLua(lsInner, -1);
        auto result = instance->GetBoot();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetArmor", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = Character::ReadFromLua(lsInner, -1);
        auto result = instance->GetArmor();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void Character::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Character::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -1);
    } else if (0 == strcmp("bootId", key)) {
      LuaHelper::Read(instance->mBootId, ls, -1);
    } else if (0 == strcmp("armorId", key)) {
      LuaHelper::Read(instance->mArmorId, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void Character::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "CharacterMeta");
}
}
