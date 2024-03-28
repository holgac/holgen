#include "TestContainerMap.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::map<uint32_t, TestContainerInnerStructWithId>& TestContainerMap::GetInnerStructsWithId() const {
  return mInnerStructsWithId;
}
std::map<uint32_t, TestContainerInnerStructWithId>& TestContainerMap::GetInnerStructsWithId() {
  return mInnerStructsWithId;
}
void TestContainerMap::SetInnerStructsWithId(const std::map<uint32_t, TestContainerInnerStructWithId>& val) {
  mInnerStructsWithId = val;
}
const TestContainerInnerStructWithId* TestContainerMap::GetInnerStructWithIdFromName(const std::string& key) const {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}
TestContainerInnerStructWithId* TestContainerMap::GetInnerStructWithIdFromName(const std::string& key) {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}
bool TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId&& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mInnerStructsWithId.emplace(newId, std::forward<TestContainerInnerStructWithId>(elem));
  return true;
}
bool TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mInnerStructsWithId.emplace(newId, elem);
  return true;
}
const TestContainerInnerStructWithId* TestContainerMap::GetInnerStructWithId(uint32_t idx) const {
  auto it = mInnerStructsWithId.find(idx);
  if (it == mInnerStructsWithId.end())
    return nullptr;
  return &it->second;
}
TestContainerInnerStructWithId* TestContainerMap::GetInnerStructWithId(uint32_t idx) {
  auto it = mInnerStructsWithId.find(idx);
  if (it == mInnerStructsWithId.end())
    return nullptr;
  return &it->second;
}
bool TestContainerMap::HasInnerStructWithId(uint32_t key) const {
  return mInnerStructsWithId.contains(key);
}
void TestContainerMap::DeleteInnerStructWithId(uint32_t key) {
  auto ptr = GetInnerStructWithId(key);
  mInnerStructsWithIdNameIndex.erase(ptr->GetName());
  mInnerStructsWithId.erase(key);
}
size_t TestContainerMap::GetInnerStructWithIdCount() const {
  return mInnerStructsWithId.size();
}
bool TestContainerMap::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "innerStructsWithId")) {
      auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void TestContainerMap::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerMapMeta");
  lua_setmetatable(luaState, -2);
}
void TestContainerMap::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestContainerMap* TestContainerMap::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerMap*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestContainerMap::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerMap::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Push(instance->mInnerStructsWithId, ls);
    } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerMap::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructWithIdFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetInnerStructWithId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerMap::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructWithId(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("HasInnerStructWithId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerMap::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->HasInnerStructWithId(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteInnerStructWithId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerMap::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteInnerStructWithId(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerMap::ReadFromLua(lsInner, -1);
        auto result = instance->GetInnerStructWithIdCount();
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
void TestContainerMap::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerMap::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Read(instance->mInnerStructsWithId, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestContainerMap::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestContainerMapMeta");
}
}