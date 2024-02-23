#include "GameData.h"

#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<Boot>& GameData::GetBoots() const {
  return mBoots;
}
std::vector<Boot>& GameData::GetBoots() {
  return mBoots;
}
void GameData::SetBoots(const std::vector<Boot>& val) {
  mBoots = val;
}
const Boot* GameData::GetBootFromName(const std::string& key) const {
  auto it = mBootsNameIndex.find(key);
  if (it == mBootsNameIndex.end())
    return nullptr;
  return &mBoots[it->second];
}
Boot* GameData::GetBootFromName(const std::string& key) {
  auto it = mBootsNameIndex.find(key);
  if (it == mBootsNameIndex.end())
    return nullptr;
  return &mBoots[it->second];
}
bool GameData::AddBoot(Boot&& elem) {
  auto newId = mBoots.size();
  if(mBootsNameIndex.contains(elem.GetName()))
    return false;
  mBootsNameIndex.emplace(elem.GetName(), newId);
  mBoots.emplace_back(std::forward<Boot>(elem));
  mBoots.back().SetId(newId);
  return true;
}
const Boot& GameData::GetBoot(uint32_t idx) const {
  return mBoots[idx];
}
Boot& GameData::GetBoot(uint32_t idx) {
  return mBoots[idx];
}
const std::vector<Armor>& GameData::GetArmors() const {
  return mArmors;
}
std::vector<Armor>& GameData::GetArmors() {
  return mArmors;
}
void GameData::SetArmors(const std::vector<Armor>& val) {
  mArmors = val;
}
const Armor* GameData::GetArmorFromName(const std::string& key) const {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}
Armor* GameData::GetArmorFromName(const std::string& key) {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}
const Armor* GameData::GetArmorFromAlternativeName(const std::string& key) const {
  auto it = mArmorsAlternativeNameIndex.find(key);
  if (it == mArmorsAlternativeNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}
Armor* GameData::GetArmorFromAlternativeName(const std::string& key) {
  auto it = mArmorsAlternativeNameIndex.find(key);
  if (it == mArmorsAlternativeNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}
bool GameData::AddArmor(Armor&& elem) {
  auto newId = mArmors.size();
  if(mArmorsNameIndex.contains(elem.GetName()))
    return false;
  if(mArmorsAlternativeNameIndex.contains(elem.GetAlternativeName()))
    return false;
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  mArmorsAlternativeNameIndex.emplace(elem.GetAlternativeName(), newId);
  mArmors.emplace_back(std::forward<Armor>(elem));
  mArmors.back().SetId(newId);
  return true;
}
const Armor& GameData::GetArmor(uint32_t idx) const {
  return mArmors[idx];
}
Armor& GameData::GetArmor(uint32_t idx) {
  return mArmors[idx];
}
const std::vector<Character>& GameData::GetCharacters() const {
  return mCharacters;
}
std::vector<Character>& GameData::GetCharacters() {
  return mCharacters;
}
void GameData::SetCharacters(const std::vector<Character>& val) {
  mCharacters = val;
}
const Character* GameData::GetCharacterFromName(const std::string& key) const {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}
Character* GameData::GetCharacterFromName(const std::string& key) {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}
bool GameData::AddCharacter(Character&& elem) {
  auto newId = mCharacters.size();
  if(mCharactersNameIndex.contains(elem.GetName()))
    return false;
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  mCharacters.emplace_back(std::forward<Character>(elem));
  mCharacters.back().SetId(newId);
  return true;
}
const Character& GameData::GetCharacter(uint32_t idx) const {
  return mCharacters[idx];
}
Character& GameData::GetCharacter(uint32_t idx) {
  return mCharacters[idx];
}
bool GameData::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "boots")) {
      auto res = JsonHelper::Parse(mBoots, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "armors")) {
      auto res = JsonHelper::Parse(mArmors, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "characters")) {
      auto res = JsonHelper::Parse(mCharacters, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void GameData::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "GameDataMeta");
  lua_setmetatable(luaState, -2);
}
void GameData::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (GameData*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("boots", key)) {
      LuaHelper::Push(instance->mBoots, ls);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Push(instance->mArmors, ls);
    } else if (0 == strcmp("characters", key)) {
      LuaHelper::Push(instance->mCharacters, ls);
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
    auto instance = (GameData*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("boots", key)) {
      LuaHelper::Read(instance->mBoots, ls, -2);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Read(instance->mArmors, ls, -2);
    } else if (0 == strcmp("characters", key)) {
      LuaHelper::Read(instance->mCharacters, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "GameDataMeta");
}
}
