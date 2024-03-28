#include "HumanManager.h"

#include <filesystem>
#include <queue>
#include <vector>
#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "FilesystemHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::unordered_map<uint32_t, Human>& HumanManager::GetHumans() const {
  return mHumans;
}
std::unordered_map<uint32_t, Human>& HumanManager::GetHumans() {
  return mHumans;
}
void HumanManager::SetHumans(const std::unordered_map<uint32_t, Human>& val) {
  mHumans = val;
}
const Human* HumanManager::GetHumanFromName(const std::string& key) const {
  auto it = mHumansNameIndex.find(key);
  if (it == mHumansNameIndex.end())
    return nullptr;
  return &mHumans.at(it->second);
}
Human* HumanManager::GetHumanFromName(const std::string& key) {
  auto it = mHumansNameIndex.find(key);
  if (it == mHumansNameIndex.end())
    return nullptr;
  return &mHumans.at(it->second);
}
bool HumanManager::AddHuman(Human&& elem) {
  if (mHumansNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Human with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mHumansNextId;
  ++mHumansNextId;
  mHumansNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mHumans.emplace(newId, std::forward<Human>(elem));
  return true;
}
bool HumanManager::AddHuman(Human& elem) {
  if (mHumansNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("Human with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mHumansNextId;
  ++mHumansNextId;
  mHumansNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mHumans.emplace(newId, elem);
  return true;
}
const Human* HumanManager::GetHuman(uint32_t idx) const {
  auto it = mHumans.find(idx);
  if (it == mHumans.end())
    return nullptr;
  return &it->second;
}
Human* HumanManager::GetHuman(uint32_t idx) {
  auto it = mHumans.find(idx);
  if (it == mHumans.end())
    return nullptr;
  return &it->second;
}
bool HumanManager::HasHuman(uint32_t key) const {
  return mHumans.contains(key);
}
void HumanManager::DeleteHuman(uint32_t key) {
  auto ptr = GetHuman(key);
  mHumansNameIndex.erase(ptr->GetName());
  mHumans.erase(key);
}
size_t HumanManager::GetHumanCount() const {
  return mHumans.size();
}
bool HumanManager::ParseFiles(const std::string& rootPath, const Converter& converterArg) {
  auto converter = converterArg;
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
  return true;
}
void HumanManager::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "HumanManagerMeta");
  lua_setmetatable(luaState, -2);
}
void HumanManager::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
HumanManager* HumanManager::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (HumanManager*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void HumanManager::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = HumanManager::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("humans", key)) {
      LuaHelper::Push(instance->mHumans, ls);
    } else if (0 == strcmp("GetHumanFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = HumanManager::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetHumanFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetHuman", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = HumanManager::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetHuman(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("HasHuman", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = HumanManager::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->HasHuman(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteHuman", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = HumanManager::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteHuman(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetHumanCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = HumanManager::ReadFromLua(lsInner, -1);
        auto result = instance->GetHumanCount();
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
void HumanManager::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = HumanManager::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("humans", key)) {
      LuaHelper::Read(instance->mHumans, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void HumanManager::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "HumanManagerMeta");
}
}
