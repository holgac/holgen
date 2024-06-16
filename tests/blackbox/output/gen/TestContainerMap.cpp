// This file is generated by holgen. Do not modify manually.
#include "TestContainerMap.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerMap::operator==(const TestContainerMap &rhs) const {
  return
      mInnerStructsWithId == rhs.mInnerStructsWithId;
}

const std::map<uint32_t, TestContainerInnerStructWithId> &TestContainerMap::GetInnerStructsWithId() const {
  return mInnerStructsWithId;
}

std::map<uint32_t, TestContainerInnerStructWithId> &TestContainerMap::GetInnerStructsWithId() {
  return mInnerStructsWithId;
}

void TestContainerMap::SetInnerStructsWithId(const std::map<uint32_t, TestContainerInnerStructWithId> &val) {
  mInnerStructsWithId = val;
}

const TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithIdFromName(const std::string &key) const {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}

TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithIdFromName(const std::string &key) {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}

TestContainerInnerStructWithId *TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId &&elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  auto[it, res] = mInnerStructsWithId.emplace(newId, std::forward<TestContainerInnerStructWithId>(elem));
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Corrupt internal ID counter - was TestContainerMap.innerStructsWithId modified externally?");
  return &(it->second);
}

TestContainerInnerStructWithId *TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId &elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  auto[it, res] = mInnerStructsWithId.emplace(newId, elem);
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Corrupt internal ID counter - was TestContainerMap.innerStructsWithId modified externally?");
  return &(it->second);
}

const TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithId(uint32_t idx) const {
  auto it = mInnerStructsWithId.find(idx);
  if (it == mInnerStructsWithId.end())
    return nullptr;
  return &it->second;
}

TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithId(uint32_t idx) {
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

bool TestContainerMap::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestContainerMap");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("innerStructsWithId", name)) {
      auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerMap.innerStructsWithId field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestContainerMap: {}", name);
    }
  }
  return true;
}

void TestContainerMap::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerMapMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerMap::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerMap *TestContainerMap::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerMap *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerMap TestContainerMap::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerMap{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Read(result.mInnerStructsWithId, luaState, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerMap::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("innerStructsWithId", key)) {
    LuaHelper::Push(instance->mInnerStructsWithId, luaState);
  } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -2);
      std::string arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructWithIdFromName(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("AddInnerStructWithId", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -2);
      auto arg0 = TestContainerInnerStructWithId::ReadProxyFromLua(lsInner, -1);
      auto result = instance->AddInnerStructWithId(*arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetInnerStructWithId", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -2);
      uint32_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructWithId(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("HasInnerStructWithId", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -2);
      uint32_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->HasInnerStructWithId(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("DeleteInnerStructWithId", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -2);
      uint32_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      instance->DeleteInnerStructWithId(arg0);
      return 0;
    });
  } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = TestContainerMap::ReadProxyFromLua(lsInner, -1);
      auto result = instance->GetInnerStructWithIdCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
    return 0;
  }
  return 1;
}

int TestContainerMap::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("innerStructsWithId", key)) {
    LuaHelper::Read(instance->mInnerStructsWithId, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
  }
  return 0;
}

void TestContainerMap::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerMap::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerMap::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerMapMeta");
}
}
