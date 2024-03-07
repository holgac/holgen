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
void Character::SetId(uint32_t val) {
  mId = val;
}
const std::string& Character::GetName() const {
  return mName;
}
std::string& Character::GetName() {
  return mName;
}
void Character::SetName(const std::string& val) {
  mName = val;
}
const Boot* Character::GetBoot() const {
  return Boot::Get(mBootId);
}
Boot* Character::GetBoot() {
  return Boot::Get(mBootId);
}
uint32_t Character::GetBootId() const {
  return mBootId;
}
void Character::SetBootId(uint32_t val) {
  mBootId = val;
}
const Armor* Character::GetArmor() const {
  return Armor::Get(mArmorId);
}
Armor* Character::GetArmor() {
  return Armor::Get(mArmorId);
}
uint32_t Character::GetArmorId() const {
  return mArmorId;
}
void Character::SetArmorId(uint32_t val) {
  mArmorId = val;
}
Character* Character::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
}
Character* Character::GetFromName(const std::string& val) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacterFromName(val);
}
bool Character::ParseJson(const rapidjson::Value& json, const Converter& converter) {
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
    } else if (0 == strcmp(name, "boot")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mBootId = converter.bootNameToId(temp);
    } else if (0 == strcmp(name, "armor")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mArmorId = converter.armorNameToId(temp);
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
void Character::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "i");
    lua_gettable(ls, -3);
    uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(ls, -1));
    auto instance = Character::Get(id);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("bootId", key)) {
      LuaHelper::Push(instance->mBootId, ls);
    } else if (0 == strcmp("boot", key)) {
      auto ptr = Boot::Get(instance->mBootId);
      if (ptr) {
        LuaHelper::Push(*ptr, ls);
      } else {
        LuaHelper::Push(nullptr, ls);
      }
    } else if (0 == strcmp("armorId", key)) {
      LuaHelper::Push(instance->mArmorId, ls);
    } else if (0 == strcmp("armor", key)) {
      auto ptr = Armor::Get(instance->mArmorId);
      if (ptr) {
        LuaHelper::Push(*ptr, ls);
      } else {
        LuaHelper::Push(nullptr, ls);
      }
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "i");
    lua_gettable(ls, -4);
    uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(ls, -1));
    auto instance = Character::Get(id);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -2);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -2);
    } else if (0 == strcmp("bootId", key)) {
      LuaHelper::Read(instance->mBootId, ls, -2);
    } else if (0 == strcmp("armorId", key)) {
      LuaHelper::Read(instance->mArmorId, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CharacterMeta");
}
}
