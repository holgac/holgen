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
const std::string& Armor::GetName() const {
  return mName;
}
std::string& Armor::GetName() {
  return mName;
}
const std::string& Armor::GetAlternativeName() const {
  return mAlternativeName;
}
std::string& Armor::GetAlternativeName() {
  return mAlternativeName;
}
int8_t Armor::GetArmorClass() const {
  return mArmorClass;
}
void Armor::SetId(uint32_t val) {
  mId = val;
}
void Armor::SetName(const std::string& val) {
  mName = val;
}
void Armor::SetAlternativeName(const std::string& val) {
  mAlternativeName = val;
}
void Armor::SetArmorClass(int8_t val) {
  mArmorClass = val;
}
Armor* Armor::Get(uint32_t id) {
  return GlobalPointer<GameData>::GetInstance()->GetArmor(id);
}
Armor* Armor::GetFromName(const std::string& val) {
  return GlobalPointer<GameData>::GetInstance()->GetArmorFromName(val);
}
Armor* Armor::GetFromAlternativeName(const std::string& val) {
  return GlobalPointer<GameData>::GetInstance()->GetArmorFromAlternativeName(val);
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
Armor* Armor::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = Armor::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}
void Armor::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Armor::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
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
    auto instance = Armor::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(instance->mId, ls, -1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(instance->mName, ls, -1);
    } else if (0 == strcmp("alternativeName", key)) {
      LuaHelper::Read(instance->mAlternativeName, ls, -1);
    } else if (0 == strcmp("armorClass", key)) {
      LuaHelper::Read(instance->mArmorClass, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "ArmorMeta");
}
}
