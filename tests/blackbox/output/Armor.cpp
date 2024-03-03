#include "Armor.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "GlobalPointer.h"
#include "GameData.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t Armor::GetId() const {
  return mId;
}
void Armor::SetId(uint32_t val) {
  mId = val;
}
const std::string& Armor::GetName() const {
  return mName;
}
std::string& Armor::GetName() {
  return mName;
}
void Armor::SetName(const std::string& val) {
  mName = val;
}
const std::string& Armor::GetAlternativeName() const {
  return mAlternativeName;
}
std::string& Armor::GetAlternativeName() {
  return mAlternativeName;
}
void Armor::SetAlternativeName(const std::string& val) {
  mAlternativeName = val;
}
int8_t Armor::GetArmorClass() const {
  return mArmorClass;
}
void Armor::SetArmorClass(int8_t val) {
  mArmorClass = val;
}
Armor* Armor::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetArmor(id);
}
bool Armor::ParseJson(const rapidjson::Value& json, const Converter& converter) {
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
    } else if (0 == strcmp(name, "alternativeName")) {
      auto res = JsonHelper::Parse(mAlternativeName, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "armorClass")) {
      auto res = JsonHelper::Parse(mArmorClass, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void Armor::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "ArmorMeta");
  lua_setmetatable(luaState, -2);
}
void Armor::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "i");
    lua_gettable(ls, -3);
    uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(ls, -1));
    auto instance = GlobalPointer<GameData>::GetInstance()->GetArmor(id);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Push(instance->mId, ls);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Push(instance->mName, ls);
    } else if (0 == strcmp("alternativeName", key)) {
      LuaHelper::Push(instance->mAlternativeName, ls);
    } else if (0 == strcmp("armorClass", key)) {
      LuaHelper::Push(instance->mArmorClass, ls);
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
    auto instance = (Armor*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -2);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -2);
    } else if (0 == strcmp("alternativeName", key)) {
      LuaHelper::Read(instance->mAlternativeName, ls, -2);
    } else if (0 == strcmp("armorClass", key)) {
      LuaHelper::Read(instance->mArmorClass, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "ArmorMeta");
}
}
