// This file is generated by holgen. Do not modify manually.
#include "TestJsonTagManager.h"

#include <filesystem>
#include <queue>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "FilesystemHelper.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestJsonTagManager::operator==(const TestJsonTagManager &rhs) const {
  return
      mTags == rhs.mTags;
}

const std::vector<TestJsonTag> &TestJsonTagManager::GetTags() const {
  return mTags;
}

std::vector<TestJsonTag> &TestJsonTagManager::GetTags() {
  return mTags;
}

void TestJsonTagManager::SetTags(const std::vector<TestJsonTag> &val) {
  mTags = val;
}

const TestJsonTag *TestJsonTagManager::GetTagFromName(const std::string &key) const {
  auto it = mTagsNameIndex.find(key);
  if (it == mTagsNameIndex.end())
    return nullptr;
  return &mTags[it->second];
}

TestJsonTag *TestJsonTagManager::GetTagFromName(const std::string &key) {
  auto it = mTagsNameIndex.find(key);
  if (it == mTagsNameIndex.end())
    return nullptr;
  return &mTags[it->second];
}

TestJsonTag *TestJsonTagManager::AddTag(TestJsonTag &&elem) {
  if (mTagsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestJsonTag with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mTags.size();
  mTagsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mTags.emplace_back(std::forward<TestJsonTag>(elem)));
}

TestJsonTag *TestJsonTagManager::AddTag(TestJsonTag &elem) {
  if (mTagsNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestJsonTag with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mTags.size();
  mTagsNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mTags.emplace_back(elem));
}

const TestJsonTag *TestJsonTagManager::GetTag(uint64_t idx) const {
  if (idx >= mTags.size())
    return nullptr;
  return &mTags[idx];
}

TestJsonTag *TestJsonTagManager::GetTag(uint64_t idx) {
  if (idx >= mTags.size())
    return nullptr;
  return &mTags[idx];
}

size_t TestJsonTagManager::GetTagCount() const {
  return mTags.size();
}

bool TestJsonTagManager::ParseFiles(const std::string &rootPath, const Converter &converterArg) {
  auto &converter = converterArg;
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
  auto it = filesByName.find("tags");
  if (it != filesByName.end()) {
    for (const auto& filePath: it->second) {
      auto contents = FilesystemHelper::ReadFile(filePath);
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of TestJsonTag entries", filePath.string());
      for (auto& jsonElem: doc.GetArray()) {
        HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
        TestJsonTag elem;
        auto res = elem.ParseJson(jsonElem, converter);
        HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
        AddTag(std::move(elem));
      }
    }
  }
  return true;
}

void TestJsonTagManager::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonTagManagerMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonTagManager::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonTagManager *TestJsonTagManager::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonTagManager *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonTagManager TestJsonTagManager::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonTagManager{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("tags", key)) {
      LuaHelper::Read(result.mTags, luaState, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonTagManager.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonTagManager::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("tags", key)) {
    LuaHelper::Push(instance->mTags, luaState);
  } else if (0 == strcmp("GetOrInsert", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestJsonTagManager::ReadProxyFromLua(lsInner, -2);
      std::string arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetOrInsert(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetTagFromName", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestJsonTagManager::ReadProxyFromLua(lsInner, -2);
      std::string arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetTagFromName(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("AddTag", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestJsonTagManager::ReadProxyFromLua(lsInner, -2);
      auto arg0 = TestJsonTag::ReadProxyFromLua(lsInner, -1);
      auto result = instance->AddTag(*arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetTag", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestJsonTagManager::ReadProxyFromLua(lsInner, -2);
      uint64_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetTag(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetTagCount", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestJsonTagManager::ReadProxyFromLua(lsInner, -1);
      auto result = instance->GetTagCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTagManager.{}", key);
    return 0;
  }
  return 1;
}

int TestJsonTagManager::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("tags", key)) {
    LuaHelper::Read(instance->mTags, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTagManager.{}", key);
  }
  return 0;
}

void TestJsonTagManager::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonTagManager::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonTagManager::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonTagManagerMeta");
}
}
