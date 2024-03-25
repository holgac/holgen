#include "GameData.h"

#include <filesystem>
#include <queue>
#include <vector>
#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "FilesystemHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<Boot>& GameData::GetBoots() const {
  return mBoots;
}
std::vector<Boot>& GameData::GetBoots() {
  return mBoots;
}
const std::vector<Armor>& GameData::GetArmors() const {
  return mArmors;
}
std::vector<Armor>& GameData::GetArmors() {
  return mArmors;
}
const std::vector<Character>& GameData::GetCharacters() const {
  return mCharacters;
}
std::vector<Character>& GameData::GetCharacters() {
  return mCharacters;
}
void GameData::SetBoots(const std::vector<Boot>& val) {
  mBoots = val;
}
void GameData::SetArmors(const std::vector<Armor>& val) {
  mArmors = val;
}
void GameData::SetCharacters(const std::vector<Character>& val) {
  mCharacters = val;
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
  if (mBootsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Boot with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mBoots.size();
  mBootsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mBoots.emplace_back(std::forward<Boot>(elem));
  return true;
}
const Boot* GameData::GetBoot(uint32_t idx) const {
  if (idx >= mBoots.size())
    return nullptr;
  return &mBoots[idx];
}
Boot* GameData::GetBoot(uint32_t idx) {
  if (idx >= mBoots.size())
    return nullptr;
  return &mBoots[idx];
}
size_t GameData::GetBootCount() const {
  return mBoots.size();
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
  if (mArmorsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Armor with name={} already exists!", elem.GetName());
    return false;
  }
  if (mArmorsAlternativeNameIndex.contains(elem.GetAlternativeName())) {
    HOLGEN_WARN("Armor with alternativeName={} already exists!", elem.GetAlternativeName());
    return false;
  }
  auto newId = mArmors.size();
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  mArmorsAlternativeNameIndex.emplace(elem.GetAlternativeName(), newId);
  elem.SetId(newId);
  mArmors.emplace_back(std::forward<Armor>(elem));
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
size_t GameData::GetArmorCount() const {
  return mArmors.size();
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
  if (mCharactersNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Character with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mCharacters.size();
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mCharacters.emplace_back(std::forward<Character>(elem));
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
size_t GameData::GetCharacterCount() const {
  return mCharacters.size();
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
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Boot entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Boot elem;
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        AddBoot(std::move(elem));
      }
    }
  }
  it = filesByName.find("armors");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Armor entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Armor elem;
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        AddArmor(std::move(elem));
      }
    }
  }
  it = filesByName.find("characters");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Character entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Character elem;
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
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
void GameData::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
GameData* GameData::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (GameData*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void GameData::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = GameData::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("boots", key)) {
      LuaHelper::Push(instance->mBoots, ls);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Push(instance->mArmors, ls);
    } else if (0 == strcmp("characters", key)) {
      LuaHelper::Push(instance->mCharacters, ls);
    } else if (0 == strcmp("GetBootFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetBootFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetBoot", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetBoot(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetBootCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -1);
        auto result = instance->GetBootCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetArmorFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetArmorFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetArmorFromAlternativeName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetArmorFromAlternativeName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetArmor", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetArmor(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetArmorCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -1);
        auto result = instance->GetArmorCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetCharacterFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetCharacterFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetCharacter", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetCharacter(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetCharacterCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = GameData::ReadFromLua(lsInner, -1);
        auto result = instance->GetCharacterCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = GameData::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("boots", key)) {
      LuaHelper::Read(instance->mBoots, ls, -1);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Read(instance->mArmors, ls, -1);
    } else if (0 == strcmp("characters", key)) {
      LuaHelper::Read(instance->mCharacters, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "GameDataMeta");
}
}
