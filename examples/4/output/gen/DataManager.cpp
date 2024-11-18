// This file is generated by holgen. Do not modify manually.
#include "DataManager.h"

#include <cstring>
#include <filesystem>
#include <queue>
#include <vector>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "FilesystemHelper.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex4_schemas {
DataManager::DataManager(DataManager &&rhs) noexcept {
  mCharacters = std::move(rhs.mCharacters);
  mArmors = std::move(rhs.mArmors);
  mWeapons = std::move(rhs.mWeapons);
  mCharactersNameIndex = std::move(rhs.mCharactersNameIndex);
  mArmorsNameIndex = std::move(rhs.mArmorsNameIndex);
  mWeaponsNameIndex = std::move(rhs.mWeaponsNameIndex);
}

const std::deque<Character> &DataManager::GetCharacters() const {
  return mCharacters;
}

std::deque<Character> &DataManager::GetCharacters() {
  return mCharacters;
}

const std::deque<Armor> &DataManager::GetArmors() const {
  return mArmors;
}

std::deque<Armor> &DataManager::GetArmors() {
  return mArmors;
}

const std::deque<Weapon> &DataManager::GetWeapons() const {
  return mWeapons;
}

std::deque<Weapon> &DataManager::GetWeapons() {
  return mWeapons;
}

void DataManager::SetCharacters(const std::deque<Character> &val) {
  mCharacters = val;
}

void DataManager::SetArmors(const std::deque<Armor> &val) {
  mArmors = val;
}

void DataManager::SetWeapons(const std::deque<Weapon> &val) {
  mWeapons = val;
}

const Character *DataManager::GetCharacterFromName(const std::string &key) const {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}

Character *DataManager::GetCharacterFromName(const std::string &key) {
  auto it = mCharactersNameIndex.find(key);
  if (it == mCharactersNameIndex.end())
    return nullptr;
  return &mCharacters[it->second];
}

Character *DataManager::AddCharacter(Character &&elem) {
  if (mCharactersNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Character with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mCharacters.size();
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mCharacters.emplace_back(std::forward<Character>(elem)));
}

Character *DataManager::AddCharacter(Character &elem) {
  if (mCharactersNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Character with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mCharacters.size();
  mCharactersNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mCharacters.emplace_back(elem));
}

const Character *DataManager::GetCharacter(uint32_t idx) const {
  if (idx >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}

Character *DataManager::GetCharacter(uint32_t idx) {
  if (idx >= mCharacters.size())
    return nullptr;
  return &mCharacters[idx];
}

void DataManager::DeleteCharacter(uint32_t idx) {
  auto ptr = GetCharacter(idx);
  mCharactersNameIndex.erase(ptr->GetName());
  if (idx != mCharacters.size() - 1) {
    mCharactersNameIndex.at(mCharacters.back().GetName()) = idx;
    mCharacters[idx] = std::move(mCharacters.back());
  }
  mCharacters.pop_back();
}

size_t DataManager::GetCharacterCount() const {
  return mCharacters.size();
}

const Armor *DataManager::GetArmorFromName(const std::string &key) const {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

Armor *DataManager::GetArmorFromName(const std::string &key) {
  auto it = mArmorsNameIndex.find(key);
  if (it == mArmorsNameIndex.end())
    return nullptr;
  return &mArmors[it->second];
}

Armor *DataManager::AddArmor(Armor &&elem) {
  if (mArmorsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Armor with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mArmors.size();
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mArmors.emplace_back(std::forward<Armor>(elem)));
}

Armor *DataManager::AddArmor(Armor &elem) {
  if (mArmorsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Armor with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mArmors.size();
  mArmorsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mArmors.emplace_back(elem));
}

const Armor *DataManager::GetArmor(uint32_t idx) const {
  if (idx >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
}

Armor *DataManager::GetArmor(uint32_t idx) {
  if (idx >= mArmors.size())
    return nullptr;
  return &mArmors[idx];
}

void DataManager::DeleteArmor(uint32_t idx) {
  auto ptr = GetArmor(idx);
  mArmorsNameIndex.erase(ptr->GetName());
  if (idx != mArmors.size() - 1) {
    mArmorsNameIndex.at(mArmors.back().GetName()) = idx;
    mArmors[idx] = std::move(mArmors.back());
  }
  mArmors.pop_back();
}

size_t DataManager::GetArmorCount() const {
  return mArmors.size();
}

const Weapon *DataManager::GetWeaponFromName(const std::string &key) const {
  auto it = mWeaponsNameIndex.find(key);
  if (it == mWeaponsNameIndex.end())
    return nullptr;
  return &mWeapons[it->second];
}

Weapon *DataManager::GetWeaponFromName(const std::string &key) {
  auto it = mWeaponsNameIndex.find(key);
  if (it == mWeaponsNameIndex.end())
    return nullptr;
  return &mWeapons[it->second];
}

Weapon *DataManager::AddWeapon(Weapon &&elem) {
  if (mWeaponsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Weapon with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mWeapons.size();
  mWeaponsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mWeapons.emplace_back(std::forward<Weapon>(elem)));
}

Weapon *DataManager::AddWeapon(Weapon &elem) {
  if (mWeaponsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Weapon with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mWeapons.size();
  mWeaponsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mWeapons.emplace_back(elem));
}

const Weapon *DataManager::GetWeapon(uint32_t idx) const {
  if (idx >= mWeapons.size())
    return nullptr;
  return &mWeapons[idx];
}

Weapon *DataManager::GetWeapon(uint32_t idx) {
  if (idx >= mWeapons.size())
    return nullptr;
  return &mWeapons[idx];
}

void DataManager::DeleteWeapon(uint32_t idx) {
  auto ptr = GetWeapon(idx);
  mWeaponsNameIndex.erase(ptr->GetName());
  if (idx != mWeapons.size() - 1) {
    mWeaponsNameIndex.at(mWeapons.back().GetName()) = idx;
    mWeapons[idx] = std::move(mWeapons.back());
  }
  mWeapons.pop_back();
}

size_t DataManager::GetWeaponCount() const {
  return mWeapons.size();
}

bool DataManager::operator==(const DataManager &rhs) const {
  return !(
      mCharacters != rhs.mCharacters ||
      mArmors != rhs.mArmors ||
      mWeapons != rhs.mWeapons ||
      mCharactersNameIndex != rhs.mCharactersNameIndex ||
      mArmorsNameIndex != rhs.mArmorsNameIndex ||
      mWeaponsNameIndex != rhs.mWeaponsNameIndex
  );
}

rapidjson::Value DataManager::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("characters", JsonHelper::Dump(mCharacters, doc), doc.GetAllocator());
  val.AddMember("armors", JsonHelper::Dump(mArmors, doc), doc.GetAllocator());
  val.AddMember("weapons", JsonHelper::Dump(mWeapons, doc), doc.GetAllocator());
  return val;
}

bool DataManager::ParseFiles(const std::string &rootPath, const Converter &converterArg) {
  auto converter = converterArg;
  if (converter.armorNameToId == nullptr) {
    converter.armorNameToId = [this](const std::string &key) -> uint32_t {
      auto elem = GetArmorFromName(key);
      HOLGEN_WARN_AND_RETURN_IF(!elem, uint32_t(-1), "{} Armor not found!", key);
      return elem->GetId();
    };
  }
  if (converter.weaponNameToId == nullptr) {
    converter.weaponNameToId = [this](const std::string &key) -> uint32_t {
      auto elem = GetWeaponFromName(key);
      HOLGEN_WARN_AND_RETURN_IF(!elem, uint32_t(-1), "{} Weapon not found!", key);
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
        std::string filename = entry.path().filename().string();
        auto dotPosition = filename.rfind('.');
        if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == "json") {
          filesByName[filename.substr(0, dotPosition)].push_back(entry.path());
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
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        AddArmor(std::move(elem));
      }
    }
  }
  it = filesByName.find("weapons");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of Weapon entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        Weapon elem;
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        AddWeapon(std::move(elem));
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

void DataManager::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "DataManager");
  lua_setmetatable(luaState, -2);
}

void DataManager::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "characters");
  LuaHelper::Push<true>(mCharacters, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armors");
  LuaHelper::Push<true>(mArmors, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "weapons");
  LuaHelper::Push<true>(mWeapons, luaState);
  lua_settable(luaState, -3);
}

void DataManager::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

DataManager *DataManager::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting DataManager", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (DataManager *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

DataManager DataManager::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = DataManager{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("characters", key)) {
      LuaHelper::Read(result.mCharacters, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armors", key)) {
      LuaHelper::Read(result.mArmors, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("weapons", key)) {
      LuaHelper::Read(result.mWeapons, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: DataManager.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int DataManager::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("characters", key)) {
    auto res = LuaHelper::Read(instance->mCharacters, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning DataManager.characters from lua failed!");
  } else if (0 == strcmp("armors", key)) {
    auto res = LuaHelper::Read(instance->mArmors, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning DataManager.armors from lua failed!");
  } else if (0 == strcmp("weapons", key)) {
    auto res = LuaHelper::Read(instance->mWeapons, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning DataManager.weapons from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: DataManager.{}", key);
  }
  return 0;
}

void DataManager::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, DataManager::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, DataManager::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "DataManager");
}

int DataManager::GetCharacterFromNameCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetCharacterFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetCharacterFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::AddCharacterCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.AddCharacter method with an invalid lua proxy object!");
  auto arg0 = Character::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddCharacter(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::GetCharacterCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetCharacter method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetCharacter(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::DeleteCharacterCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.DeleteCharacter method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteCharacter(arg0);
  return 0;
}

int DataManager::GetCharacterCountCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetCharacterCount method with an invalid lua proxy object!");
  auto result = instance->GetCharacterCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int DataManager::GetArmorFromNameCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetArmorFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetArmorFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::AddArmorCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.AddArmor method with an invalid lua proxy object!");
  auto arg0 = Armor::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddArmor(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::GetArmorCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetArmor method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetArmor(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::DeleteArmorCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.DeleteArmor method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteArmor(arg0);
  return 0;
}

int DataManager::GetArmorCountCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetArmorCount method with an invalid lua proxy object!");
  auto result = instance->GetArmorCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int DataManager::GetWeaponFromNameCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetWeaponFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetWeaponFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::AddWeaponCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.AddWeapon method with an invalid lua proxy object!");
  auto arg0 = Weapon::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddWeapon(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::GetWeaponCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetWeapon method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetWeapon(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int DataManager::DeleteWeaponCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.DeleteWeapon method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteWeapon(arg0);
  return 0;
}

int DataManager::GetWeaponCountCallerFromLua(lua_State *luaState) {
  auto instance = DataManager::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling DataManager.GetWeaponCount method with an invalid lua proxy object!");
  auto result = instance->GetWeaponCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int DataManager::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("characters", key)) {
    auto instance = DataManager::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for DataManager.characters with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mCharacters, luaState);
  } else if (0 == strcmp("armors", key)) {
    auto instance = DataManager::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for DataManager.armors with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mArmors, luaState);
  } else if (0 == strcmp("weapons", key)) {
    auto instance = DataManager::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for DataManager.weapons with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mWeapons, luaState);
  } else if (0 == strcmp("GetCharacterFromName", key)) {
    lua_pushcfunction(luaState, DataManager::GetCharacterFromNameCallerFromLua);
  } else if (0 == strcmp("AddCharacter", key)) {
    lua_pushcfunction(luaState, DataManager::AddCharacterCallerFromLua);
  } else if (0 == strcmp("GetCharacter", key)) {
    lua_pushcfunction(luaState, DataManager::GetCharacterCallerFromLua);
  } else if (0 == strcmp("DeleteCharacter", key)) {
    lua_pushcfunction(luaState, DataManager::DeleteCharacterCallerFromLua);
  } else if (0 == strcmp("GetCharacterCount", key)) {
    lua_pushcfunction(luaState, DataManager::GetCharacterCountCallerFromLua);
  } else if (0 == strcmp("GetArmorFromName", key)) {
    lua_pushcfunction(luaState, DataManager::GetArmorFromNameCallerFromLua);
  } else if (0 == strcmp("AddArmor", key)) {
    lua_pushcfunction(luaState, DataManager::AddArmorCallerFromLua);
  } else if (0 == strcmp("GetArmor", key)) {
    lua_pushcfunction(luaState, DataManager::GetArmorCallerFromLua);
  } else if (0 == strcmp("DeleteArmor", key)) {
    lua_pushcfunction(luaState, DataManager::DeleteArmorCallerFromLua);
  } else if (0 == strcmp("GetArmorCount", key)) {
    lua_pushcfunction(luaState, DataManager::GetArmorCountCallerFromLua);
  } else if (0 == strcmp("GetWeaponFromName", key)) {
    lua_pushcfunction(luaState, DataManager::GetWeaponFromNameCallerFromLua);
  } else if (0 == strcmp("AddWeapon", key)) {
    lua_pushcfunction(luaState, DataManager::AddWeaponCallerFromLua);
  } else if (0 == strcmp("GetWeapon", key)) {
    lua_pushcfunction(luaState, DataManager::GetWeaponCallerFromLua);
  } else if (0 == strcmp("DeleteWeapon", key)) {
    lua_pushcfunction(luaState, DataManager::DeleteWeaponCallerFromLua);
  } else if (0 == strcmp("GetWeaponCount", key)) {
    lua_pushcfunction(luaState, DataManager::GetWeaponCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: DataManager.{}", key);
    return 0;
  }
  return 1;
}

DataManager &DataManager::operator=(DataManager &&rhs) noexcept {
  mCharacters = std::move(rhs.mCharacters);
  mArmors = std::move(rhs.mArmors);
  mWeapons = std::move(rhs.mWeapons);
  mCharactersNameIndex = std::move(rhs.mCharactersNameIndex);
  mArmorsNameIndex = std::move(rhs.mArmorsNameIndex);
  mWeaponsNameIndex = std::move(rhs.mWeaponsNameIndex);
  return *this;
}
}
