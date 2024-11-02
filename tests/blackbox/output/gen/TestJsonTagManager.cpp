// This file is generated by holgen. Do not modify manually.
#include "TestJsonTagManager.h"

#include <cstring>
#include <filesystem>
#include <queue>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "FilesystemHelper.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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

bool TestJsonTagManager::operator==(const TestJsonTagManager &rhs) const {
  return !(
      mTags != rhs.mTags ||
      mTagsNameIndex != rhs.mTagsNameIndex
  );
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
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonTagManagerMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonTagManager::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "tags");
  LuaHelper::Push<true>(mTags, luaState);
  lua_settable(luaState, -3);
}

void TestJsonTagManager::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonTagManager *TestJsonTagManager::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestJsonTagManager", className);
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
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonTagManager.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonTagManager::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("tags", key)) {
    auto res = LuaHelper::Read(instance->mTags, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonTagManager.tags from lua failed!");
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

int TestJsonTagManager::GetOrInsertCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonTagManager.GetOrInsert method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetOrInsert(arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestJsonTagManager::GetTagFromNameCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonTagManager.GetTagFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetTagFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestJsonTagManager::AddTagCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonTagManager.AddTag method with an invalid lua proxy object!");
  auto arg0 = TestJsonTag::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddTag(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestJsonTagManager::GetTagCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonTagManager.GetTag method with an invalid lua proxy object!");
  uint64_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetTag(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestJsonTagManager::GetTagCountCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonTagManager.GetTagCount method with an invalid lua proxy object!");
  auto result = instance->GetTagCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestJsonTagManager::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("tags", key)) {
    auto instance = TestJsonTagManager::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonTagManager.tags with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTags, luaState);
  } else if (0 == strcmp("GetOrInsert", key)) {
    lua_pushcfunction(luaState, TestJsonTagManager::GetOrInsertCallerFromLua);
  } else if (0 == strcmp("GetTagFromName", key)) {
    lua_pushcfunction(luaState, TestJsonTagManager::GetTagFromNameCallerFromLua);
  } else if (0 == strcmp("AddTag", key)) {
    lua_pushcfunction(luaState, TestJsonTagManager::AddTagCallerFromLua);
  } else if (0 == strcmp("GetTag", key)) {
    lua_pushcfunction(luaState, TestJsonTagManager::GetTagCallerFromLua);
  } else if (0 == strcmp("GetTagCount", key)) {
    lua_pushcfunction(luaState, TestJsonTagManager::GetTagCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTagManager.{}", key);
    return 0;
  }
  return 1;
}
}
