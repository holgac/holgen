#include "GameData.h"

#include <filesystem>
#include <queue>
#include <vector>
#include <fstream>
#include <rapidjson/document.h>
#include <lua.hpp>
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
const Boot* GameData::GetBoot(uint32_t idx) const {
  if (idx >= mBoots.size())
    return nullptr;
  return &mBoots[idx];
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
const Armor* GameData::GetArmor(uint32_t idx) const {
  if (idx >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
}
Armor* GameData::GetArmor(uint32_t idx) {
  if (idx >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
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
const Character* GameData::GetCharacter(uint32_t idx) const {
  if (idx >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}
Character* GameData::GetCharacter(uint32_t idx) {
  if (idx >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}
bool GameData::ParseFiles(const std::string& rootPath, const Converter& converterArg) {
  auto converter = converterArg;
  if (converter.bootNameToId == nullptr) {
    converter.bootNameToId = [this](const std::string& key) -> uint32_t {
      auto elem = GetBootFromName(key);
      return elem->GetId();
    };
  }
  if (converter.armorNameToId == nullptr) {
    converter.armorNameToId = [this](const std::string& key) -> uint32_t {
      auto elem = GetArmorFromName(key);
      return elem->GetId();
    };
  }
  std::map<std::string, std::vector<std::filesystem::path>> filesByName;
  std::queue<std::filesystem::path> pathsQueue;
  pathsQueue.push(std::filesystem::path(rootPath));
  while(!pathsQueue.empty()) {
    auto& curPath = pathsQueue.front();
    for (auto &entry: std::filesystem::directory_iterator(curPath)) {
      if (std::filesystem::is_directory(entry)) {
        pathsQueue.push(entry.path());
      } else if (std::filesystem::is_regular_file(entry)) {
        std::string filename = entry.path().filename();
        auto dotPosition = filename.rfind('.');
        if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == "json") {
          filesByName[filename.substr(0, dotPosition)].push_back(entry.path());
        }
      }
    }
    pathsQueue.pop();
  }
  auto it = filesByName.find("boots");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      std::ifstream fin(filePath, std::ios_base::binary);
      fin.seekg(0, std::ios_base::end);
      std::string contents(fin.tellg(), 0);
      fin.seekg(0, std::ios_base::beg);
      fin.read(contents.data(), contents.size());
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      if (!doc.IsArray()) {
        return false;
      }
      for (auto& jsonElem: doc.GetArray()) {
        Boot elem;
        elem.ParseJson(jsonElem, converter);
        AddBoot(std::move(elem));
      }
    }
  }
  it = filesByName.find("armors");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      std::ifstream fin(filePath, std::ios_base::binary);
      fin.seekg(0, std::ios_base::end);
      std::string contents(fin.tellg(), 0);
      fin.seekg(0, std::ios_base::beg);
      fin.read(contents.data(), contents.size());
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      if (!doc.IsArray()) {
        return false;
      }
      for (auto& jsonElem: doc.GetArray()) {
        Armor elem;
        elem.ParseJson(jsonElem, converter);
        AddArmor(std::move(elem));
      }
    }
  }
  it = filesByName.find("characters");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      std::ifstream fin(filePath, std::ios_base::binary);
      fin.seekg(0, std::ios_base::end);
      std::string contents(fin.tellg(), 0);
      fin.seekg(0, std::ios_base::beg);
      fin.read(contents.data(), contents.size());
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      if (!doc.IsArray()) {
        return false;
      }
      for (auto& jsonElem: doc.GetArray()) {
        Character elem;
        elem.ParseJson(jsonElem, converter);
        AddCharacter(std::move(elem));
      }
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
