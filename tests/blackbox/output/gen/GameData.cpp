// This file is generated by holgen. Do not modify manually.
#include "GameData.h"

#include <cstring>
#include <fstream>
#include <queue>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "Converter.h"
#include "FilesystemHelper.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<Boot> &GameData::GetBoots() const {
  return mBoots;
}

std::vector<Boot> &GameData::GetBoots() {
  return mBoots;
}

const std::vector<Armor> &GameData::GetArmors() const {
  return mArmors;
}

std::vector<Armor> &GameData::GetArmors() {
  return mArmors;
}

const std::vector<Character> &GameData::GetCharacters() const {
  return mCharacters;
}

std::vector<Character> &GameData::GetCharacters() {
  return mCharacters;
}

void GameData::SetBoots(const std::vector<Boot> &val) {
  mBoots = val;
}

void GameData::SetArmors(const std::vector<Armor> &val) {
  mArmors = val;
}

void GameData::SetCharacters(const std::vector<Character> &val) {
  mCharacters = val;
}

const Boot *GameData::GetBootFromName(const std::string &key) const {
  auto it = mBootsNameIndex.find(key);
  if (it == mBootsNameIndex.end())
    return nullptr;
  return &mBoots[it->second];
}

Boot *GameData::GetBootFromName(const std::string &key) {
  auto it = mBootsNameIndex.find(key);
  if (it == mBootsNameIndex.end())
    return nullptr;
  return &mBoots[it->second];
}

const Armor *GameData::GetArmorFromName(const std::string &key) const {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

Armor *GameData::GetArmorFromName(const std::string &key) {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

const Armor *GameData::GetArmorFromAlternativeName(const std::string &key) const {
  auto it = mArmorsAlternativeNameIndex.find(key);
  if (it == mArmorsAlternativeNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

Armor *GameData::GetArmorFromAlternativeName(const std::string &key) {
  auto it = mArmorsAlternativeNameIndex.find(key);
  if (it == mArmorsAlternativeNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

const Character *GameData::GetCharacterFromName(const std::string &key) const {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}

Character *GameData::GetCharacterFromName(const std::string &key) {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}

Boot *GameData::AddBoot(Boot &&elem) {
  if (mBootsNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mBoots.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Boot::IdType(-1) && idInElem != Boot::IdType(newId), "Objects not loaded in the right order!");
  mBootsNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mBoots.emplace_back(std::move(elem));
  newElem.SetId(newId);
  return &newElem;
}

Boot *GameData::AddBoot(Boot &elem) {
  if (mBootsNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mBoots.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Boot::IdType(-1) && idInElem != Boot::IdType(newId), "Objects not loaded in the right order!");
  mBootsNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mBoots.emplace_back(elem);
  newElem.SetId(newId);
  return &newElem;
}

Armor *GameData::AddArmor(Armor &&elem) {
  if (mArmorsNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  if (mArmorsAlternativeNameIndex.contains(elem.GetAlternativeName())) {
    HOLGEN_WARN("Armor with alternativeName={} already exists", elem.GetAlternativeName());
    return nullptr;
  }
  auto newId = mArmors.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Armor::IdType(-1) && idInElem != Armor::IdType(newId), "Objects not loaded in the right order!");
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  mArmorsAlternativeNameIndex.emplace(elem.GetAlternativeName(), newId);
  auto &newElem = mArmors.emplace_back(std::move(elem));
  newElem.SetId(newId);
  return &newElem;
}

Armor *GameData::AddArmor(Armor &elem) {
  if (mArmorsNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  if (mArmorsAlternativeNameIndex.contains(elem.GetAlternativeName())) {
    HOLGEN_WARN("Armor with alternativeName={} already exists", elem.GetAlternativeName());
    return nullptr;
  }
  auto newId = mArmors.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Armor::IdType(-1) && idInElem != Armor::IdType(newId), "Objects not loaded in the right order!");
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  mArmorsAlternativeNameIndex.emplace(elem.GetAlternativeName(), newId);
  auto &newElem = mArmors.emplace_back(elem);
  newElem.SetId(newId);
  return &newElem;
}

Character *GameData::AddCharacter(Character &&elem) {
  if (mCharactersNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mCharacters.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Character::IdType(-1) && idInElem != Character::IdType(newId), "Objects not loaded in the right order!");
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mCharacters.emplace_back(std::move(elem));
  newElem.SetId(newId);
  return &newElem;
}

Character *GameData::AddCharacter(Character &elem) {
  if (mCharactersNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mCharacters.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != Character::IdType(-1) && idInElem != Character::IdType(newId), "Objects not loaded in the right order!");
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mCharacters.emplace_back(elem);
  newElem.SetId(newId);
  return &newElem;
}

const Boot *GameData::GetBoot(uint32_t idx) const {
  if (size_t(idx) >= mBoots.size())
    return nullptr;
  return &mBoots[idx];
}

Boot *GameData::GetBoot(uint32_t idx) {
  if (size_t(idx) >= mBoots.size())
    return nullptr;
  return &mBoots[idx];
}

const Armor *GameData::GetArmor(uint32_t idx) const {
  if (size_t(idx) >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
}

Armor *GameData::GetArmor(uint32_t idx) {
  if (size_t(idx) >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
}

const Character *GameData::GetCharacter(uint32_t idx) const {
  if (size_t(idx) >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}

Character *GameData::GetCharacter(uint32_t idx) {
  if (size_t(idx) >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}

void GameData::DeleteBoot(uint32_t idx) {
  auto ptr = GetBoot(idx);
  mBootsNameIndex.erase(ptr->GetName());
  if (size_t(idx) != mBoots.size() - 1) {
    mBootsNameIndex.at(mBoots.back().GetName()) = idx;
    mBoots[idx] = std::move(mBoots.back());
  }
  mBoots.pop_back();
}

void GameData::DeleteArmor(uint32_t idx) {
  auto ptr = GetArmor(idx);
  mArmorsNameIndex.erase(ptr->GetName());
  mArmorsAlternativeNameIndex.erase(ptr->GetAlternativeName());
  if (size_t(idx) != mArmors.size() - 1) {
    mArmorsNameIndex.at(mArmors.back().GetName()) = idx;
    mArmorsAlternativeNameIndex.at(mArmors.back().GetAlternativeName()) = idx;
    mArmors[idx] = std::move(mArmors.back());
  }
  mArmors.pop_back();
}

void GameData::DeleteCharacter(uint32_t idx) {
  auto ptr = GetCharacter(idx);
  mCharactersNameIndex.erase(ptr->GetName());
  if (size_t(idx) != mCharacters.size() - 1) {
    mCharactersNameIndex.at(mCharacters.back().GetName()) = idx;
    mCharacters[idx] = std::move(mCharacters.back());
  }
  mCharacters.pop_back();
}

size_t GameData::GetBootCount() const {
  return mBoots.size();
}

size_t GameData::GetArmorCount() const {
  return mArmors.size();
}

size_t GameData::GetCharacterCount() const {
  return mCharacters.size();
}

bool GameData::operator==(const GameData &rhs) const {
  return !(
      mBoots != rhs.mBoots ||
      mArmors != rhs.mArmors ||
      mCharacters != rhs.mCharacters ||
      mBootsNameIndex != rhs.mBootsNameIndex ||
      mArmorsNameIndex != rhs.mArmorsNameIndex ||
      mArmorsAlternativeNameIndex != rhs.mArmorsAlternativeNameIndex ||
      mCharactersNameIndex != rhs.mCharactersNameIndex
  );
}

bool GameData::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  return true;
}

rapidjson::Value GameData::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  return val;
}

bool GameData::ParseFiles(const std::filesystem::path &rootPath, const std::string &selfName, const Converter &converterArg, lua_State *luaState) {
  auto converter = converterArg;
  if (converter.bootNameToId == nullptr) {
    converter.bootNameToId = [this](const std::string &key) -> uint32_t {
      auto elem = GetBootFromName(key);
      HOLGEN_WARN_AND_RETURN_IF(!elem, uint32_t(-1), "{} Boot not found!", key);
      return elem->GetId();
    };
  }
  if (converter.armorNameToId == nullptr) {
    converter.armorNameToId = [this](const std::string &key) -> uint32_t {
      auto elem = GetArmorFromName(key);
      HOLGEN_WARN_AND_RETURN_IF(!elem, uint32_t(-1), "{} Armor not found!", key);
      return elem->GetId();
    };
  }
  if (!selfName.empty()) {
    auto contents = FilesystemHelper::ReadFile(rootPath / (selfName + ".json"));
    rapidjson::Document doc;
    doc.Parse(contents.c_str());
    ParseJson(doc, converter, luaState);
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
        if (entry.path().extension() == ".json") {
          auto filename = entry.path().filename().string();
          filesByName[filename.substr(0, filename.size() - 5)].push_back(entry.path());
        }
      }
    }
    pathsQueue.pop();
  }
  auto it = filesByName.find("armors");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Armor entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Armor elem;
        auto res = JsonHelper::Parse(elem, jsonElem, converter, luaState);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        auto elemPtr = AddArmor(std::move(elem));
        if (elemPtr == nullptr) {
          auto existingElem = GetArmorFromName(elem.GetName());
          HOLGEN_WARN_AND_CONTINUE_IF(existingElem != GetArmorFromAlternativeName(elem.GetAlternativeName()), "Invalid std::vector element (name={}) matching multiple indices cannot be parsed!", elem.GetName());
          JsonHelper::Parse(*existingElem, jsonElem, converter, luaState);
        }
      }
    }
  }
  it = filesByName.find("boots");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Boot entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Boot elem;
        auto res = JsonHelper::Parse(elem, jsonElem, converter, luaState);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        auto elemPtr = AddBoot(std::move(elem));
        if (elemPtr == nullptr) {
          auto existingElem = GetBootFromName(elem.GetName());
          JsonHelper::Parse(*existingElem, jsonElem, converter, luaState);
        }
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
        auto res = JsonHelper::Parse(elem, jsonElem, converter, luaState);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        auto elemPtr = AddCharacter(std::move(elem));
        if (elemPtr == nullptr) {
          auto existingElem = GetCharacterFromName(elem.GetName());
          JsonHelper::Parse(*existingElem, jsonElem, converter, luaState);
        }
      }
    }
  }
  return true;
}

void GameData::DumpFiles(const std::filesystem::path &rootPath, const std::string &selfName, lua_State *luaState) const {
  if (std::filesystem::exists(rootPath)) {
    std::filesystem::remove_all(rootPath);
  }
  std::filesystem::create_directories(rootPath);
  rapidjson::Document doc;
  JsonHelper::DumpToFile(rootPath / (selfName + ".json"), DumpJson(doc, luaState));
  JsonHelper::DumpToFile(rootPath / "boots.json", JsonHelper::Dump(mBoots, doc, luaState));
  JsonHelper::DumpToFile(rootPath / "armors.json", JsonHelper::Dump(mArmors, doc, luaState));
  JsonHelper::DumpToFile(rootPath / "characters.json", JsonHelper::Dump(mCharacters, doc, luaState));
}

void GameData::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "GameData");
  lua_setmetatable(luaState, -2);
}

void GameData::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "boots");
  LuaHelper::Push<true>(mBoots, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armors");
  LuaHelper::Push<true>(mArmors, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "characters");
  LuaHelper::Push<true>(mCharacters, luaState);
  lua_settable(luaState, -3);
}

void GameData::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

GameData *GameData::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting GameData", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (GameData *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

GameData GameData::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = GameData{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("boots", key)) {
      LuaHelper::Read(result.mBoots, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Read(result.mArmors, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("characters", key)) {
      LuaHelper::Read(result.mCharacters, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: GameData.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int GameData::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("boots", key)) {
    auto res = LuaHelper::Read(instance->mBoots, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning GameData.boots from lua failed!");
  } else if (0 == strcmp("armors", key)) {
    auto res = LuaHelper::Read(instance->mArmors, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning GameData.armors from lua failed!");
  } else if (0 == strcmp("characters", key)) {
    auto res = LuaHelper::Read(instance->mCharacters, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning GameData.characters from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: GameData.{}", key);
  }
  return 0;
}

int GameData::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.operator== method with an invalid lua proxy object!");
  GameData arg0Mirror;
  GameData *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = GameData::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = GameData::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void GameData::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, GameData::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, GameData::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, GameData::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "GameData");
}

int GameData::GetBootFromNameCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetBootFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetBootFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetArmorFromNameCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetArmorFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetArmorFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetArmorFromAlternativeNameCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetArmorFromAlternativeName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetArmorFromAlternativeName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetCharacterFromNameCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetCharacterFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetCharacterFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::AddBootCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.AddBoot method with an invalid lua proxy object!");
  auto arg0 = Boot::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddBoot(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::AddArmorCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.AddArmor method with an invalid lua proxy object!");
  auto arg0 = Armor::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddArmor(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::AddCharacterCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.AddCharacter method with an invalid lua proxy object!");
  auto arg0 = Character::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddCharacter(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetBootCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetBoot method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetBoot(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetArmorCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetArmor method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetArmor(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::GetCharacterCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetCharacter method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetCharacter(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int GameData::DeleteBootCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.DeleteBoot method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteBoot(arg0);
  return 0;
}

int GameData::DeleteArmorCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.DeleteArmor method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteArmor(arg0);
  return 0;
}

int GameData::DeleteCharacterCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.DeleteCharacter method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteCharacter(arg0);
  return 0;
}

int GameData::GetBootCountCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetBootCount method with an invalid lua proxy object!");
  auto result = instance->GetBootCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int GameData::GetArmorCountCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetArmorCount method with an invalid lua proxy object!");
  auto result = instance->GetArmorCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int GameData::GetCharacterCountCallerFromLua(lua_State *luaState) {
  auto instance = GameData::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling GameData.GetCharacterCount method with an invalid lua proxy object!");
  auto result = instance->GetCharacterCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int GameData::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("boots", key)) {
    auto instance = GameData::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for GameData.boots with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mBoots, luaState);
  } else if (0 == strcmp("armors", key)) {
    auto instance = GameData::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for GameData.armors with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mArmors, luaState);
  } else if (0 == strcmp("characters", key)) {
    auto instance = GameData::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for GameData.characters with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mCharacters, luaState);
  } else if (0 == strcmp("GetBootFromName", key)) {
    lua_pushcfunction(luaState, GameData::GetBootFromNameCallerFromLua);
  } else if (0 == strcmp("GetArmorFromName", key)) {
    lua_pushcfunction(luaState, GameData::GetArmorFromNameCallerFromLua);
  } else if (0 == strcmp("GetArmorFromAlternativeName", key)) {
    lua_pushcfunction(luaState, GameData::GetArmorFromAlternativeNameCallerFromLua);
  } else if (0 == strcmp("GetCharacterFromName", key)) {
    lua_pushcfunction(luaState, GameData::GetCharacterFromNameCallerFromLua);
  } else if (0 == strcmp("AddBoot", key)) {
    lua_pushcfunction(luaState, GameData::AddBootCallerFromLua);
  } else if (0 == strcmp("AddArmor", key)) {
    lua_pushcfunction(luaState, GameData::AddArmorCallerFromLua);
  } else if (0 == strcmp("AddCharacter", key)) {
    lua_pushcfunction(luaState, GameData::AddCharacterCallerFromLua);
  } else if (0 == strcmp("GetBoot", key)) {
    lua_pushcfunction(luaState, GameData::GetBootCallerFromLua);
  } else if (0 == strcmp("GetArmor", key)) {
    lua_pushcfunction(luaState, GameData::GetArmorCallerFromLua);
  } else if (0 == strcmp("GetCharacter", key)) {
    lua_pushcfunction(luaState, GameData::GetCharacterCallerFromLua);
  } else if (0 == strcmp("DeleteBoot", key)) {
    lua_pushcfunction(luaState, GameData::DeleteBootCallerFromLua);
  } else if (0 == strcmp("DeleteArmor", key)) {
    lua_pushcfunction(luaState, GameData::DeleteArmorCallerFromLua);
  } else if (0 == strcmp("DeleteCharacter", key)) {
    lua_pushcfunction(luaState, GameData::DeleteCharacterCallerFromLua);
  } else if (0 == strcmp("GetBootCount", key)) {
    lua_pushcfunction(luaState, GameData::GetBootCountCallerFromLua);
  } else if (0 == strcmp("GetArmorCount", key)) {
    lua_pushcfunction(luaState, GameData::GetArmorCountCallerFromLua);
  } else if (0 == strcmp("GetCharacterCount", key)) {
    lua_pushcfunction(luaState, GameData::GetCharacterCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: GameData.{}", key);
    return 0;
  }
  return 1;
}
}
