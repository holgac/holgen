// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElemContainer.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<TestStructSingleElem>& TestStructSingleElemContainer::GetSingleElemStructs() const {
  return mSingleElemStructs;
}
std::vector<TestStructSingleElem>& TestStructSingleElemContainer::GetSingleElemStructs() {
  return mSingleElemStructs;
}
const std::vector<TestStructSingleElemWithId>& TestStructSingleElemContainer::GetSingleElemStructsWithId() const {
  return mSingleElemStructsWithId;
}
std::vector<TestStructSingleElemWithId>& TestStructSingleElemContainer::GetSingleElemStructsWithId() {
  return mSingleElemStructsWithId;
}
void TestStructSingleElemContainer::SetSingleElemStructs(const std::vector<TestStructSingleElem>& val) {
  mSingleElemStructs = val;
}
void TestStructSingleElemContainer::SetSingleElemStructsWithId(const std::vector<TestStructSingleElemWithId>& val) {
  mSingleElemStructsWithId = val;
}
TestStructSingleElem* TestStructSingleElemContainer::AddSingleElemStruct(TestStructSingleElem&& elem) {
  return &(mSingleElemStructs.emplace_back(std::forward<TestStructSingleElem>(elem)));
}
TestStructSingleElem* TestStructSingleElemContainer::AddSingleElemStruct(const TestStructSingleElem& elem) {
  return &(mSingleElemStructs.emplace_back(elem));
}
const TestStructSingleElem* TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) const {
  if (idx >= mSingleElemStructs.size())
    return nullptr;
  return &mSingleElemStructs[idx];
}
TestStructSingleElem* TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) {
  if (idx >= mSingleElemStructs.size())
    return nullptr;
  return &mSingleElemStructs[idx];
}
void TestStructSingleElemContainer::DeleteSingleElemStruct(size_t idx) {
  if (idx != mSingleElemStructs.size() - 1) {
    mSingleElemStructs[idx] = std::move(mSingleElemStructs.back());
  }
  mSingleElemStructs.pop_back();
}
size_t TestStructSingleElemContainer::GetSingleElemStructCount() const {
  return mSingleElemStructs.size();
}
TestStructSingleElemWithId* TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId&& elem) {
  auto newId = mSingleElemStructsWithId.size();
  elem.SetId(newId);
  return &(mSingleElemStructsWithId.emplace_back(std::forward<TestStructSingleElemWithId>(elem)));
}
TestStructSingleElemWithId* TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId& elem) {
  auto newId = mSingleElemStructsWithId.size();
  elem.SetId(newId);
  return &(mSingleElemStructsWithId.emplace_back(elem));
}
const TestStructSingleElemWithId* TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) const {
  if (idx >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}
TestStructSingleElemWithId* TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) {
  if (idx >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}
size_t TestStructSingleElemContainer::GetSingleElemStructWithIdCount() const {
  return mSingleElemStructsWithId.size();
}
bool TestStructSingleElemContainer::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestStructSingleElemContainer");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("singleElemStructs", name)) {
      auto res = JsonHelper::Parse(mSingleElemStructs, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructs field");
    } else if (0 == strcmp("singleElemStructsWithId", name)) {
      auto res = JsonHelper::Parse(mSingleElemStructsWithId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructsWithId field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestStructSingleElemContainer: {}", name);
    }
  }
  return true;
}
void TestStructSingleElemContainer::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemContainerMeta");
  lua_setmetatable(luaState, -2);
}
void TestStructSingleElemContainer::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestStructSingleElemContainer* TestStructSingleElemContainer::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElemContainer*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestStructSingleElemContainer::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStructSingleElemContainer::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("singleElemStructs", key)) {
      LuaHelper::Push(instance->mSingleElemStructs, ls);
    } else if (0 == strcmp("singleElemStructsWithId", key)) {
      LuaHelper::Push(instance->mSingleElemStructsWithId, ls);
    } else if (0 == strcmp("GetSingleElemStruct", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestStructSingleElemContainer::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetSingleElemStruct(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteSingleElemStruct", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestStructSingleElemContainer::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteSingleElemStruct(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetSingleElemStructCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestStructSingleElemContainer::ReadFromLua(lsInner, -1);
        auto result = instance->GetSingleElemStructCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetSingleElemStructWithId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestStructSingleElemContainer::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetSingleElemStructWithId(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetSingleElemStructWithIdCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestStructSingleElemContainer::ReadFromLua(lsInner, -1);
        auto result = instance->GetSingleElemStructWithIdCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
}
void TestStructSingleElemContainer::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestStructSingleElemContainer::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("singleElemStructs", key)) {
      LuaHelper::Read(instance->mSingleElemStructs, ls, -1);
    } else if (0 == strcmp("singleElemStructsWithId", key)) {
      LuaHelper::Read(instance->mSingleElemStructsWithId, ls, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestStructSingleElemContainer::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestStructSingleElemContainerMeta");
}
}
