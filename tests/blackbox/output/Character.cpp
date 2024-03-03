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
uint32_t Character::GetBoot() const {
  return mBoot;
}
void Character::SetBoot(uint32_t val) {
  mBoot = val;
}
uint32_t Character::GetArmor() const {
  return mArmor;
}
void Character::SetArmor(uint32_t val) {
  mArmor = val;
}
Character* Character::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
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
      mBoot = converter.bootNameToId(temp);
    } else if (0 == strcmp(name, "armor")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mArmor = converter.armorNameToId(temp);
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
    auto instance = GlobalPointer<GameData>::GetInstance()->GetCharacter(id);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("boot", key)) {
      LuaHelper::Push(instance->mBoot, ls);
    } else if (0 == strcmp("armor", key)) {
      LuaHelper::Push(instance->mArmor, ls);
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
    auto instance = (Character*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -2);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -2);
    } else if (0 == strcmp("boot", key)) {
      LuaHelper::Read(instance->mBoot, ls, -2);
    } else if (0 == strcmp("armor", key)) {
      LuaHelper::Read(instance->mArmor, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CharacterMeta");
}
}
