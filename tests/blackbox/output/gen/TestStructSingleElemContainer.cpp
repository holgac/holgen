// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElemContainer.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructSingleElemContainer::operator==(const TestStructSingleElemContainer &rhs) const {
  return !(
      mSingleElemStructs != rhs.mSingleElemStructs ||
      mSingleElemStructsWithId != rhs.mSingleElemStructsWithId
  );
}

const std::vector<TestStructSingleElem> &TestStructSingleElemContainer::GetSingleElemStructs() const {
  return mSingleElemStructs;
}

std::vector<TestStructSingleElem> &TestStructSingleElemContainer::GetSingleElemStructs() {
  return mSingleElemStructs;
}

const std::vector<TestStructSingleElemWithId> &TestStructSingleElemContainer::GetSingleElemStructsWithId() const {
  return mSingleElemStructsWithId;
}

std::vector<TestStructSingleElemWithId> &TestStructSingleElemContainer::GetSingleElemStructsWithId() {
  return mSingleElemStructsWithId;
}

void TestStructSingleElemContainer::SetSingleElemStructs(const std::vector<TestStructSingleElem> &val) {
  mSingleElemStructs = val;
}

void TestStructSingleElemContainer::SetSingleElemStructsWithId(const std::vector<TestStructSingleElemWithId> &val) {
  mSingleElemStructsWithId = val;
}

TestStructSingleElem *TestStructSingleElemContainer::AddSingleElemStruct(TestStructSingleElem &&elem) {
  return &(mSingleElemStructs.emplace_back(std::forward<TestStructSingleElem>(elem)));
}

TestStructSingleElem *TestStructSingleElemContainer::AddSingleElemStruct(const TestStructSingleElem &elem) {
  return &(mSingleElemStructs.emplace_back(elem));
}

const TestStructSingleElem *TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) const {
  if (idx >= mSingleElemStructs.size())
    return nullptr;
  return &mSingleElemStructs[idx];
}

TestStructSingleElem *TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) {
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

TestStructSingleElemWithId *TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId &&elem) {
  auto newId = mSingleElemStructsWithId.size();
  elem.SetId(newId);
  return &(mSingleElemStructsWithId.emplace_back(std::forward<TestStructSingleElemWithId>(elem)));
}

TestStructSingleElemWithId *TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId &elem) {
  auto newId = mSingleElemStructsWithId.size();
  elem.SetId(newId);
  return &(mSingleElemStructsWithId.emplace_back(elem));
}

const TestStructSingleElemWithId *TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) const {
  if (idx >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}

TestStructSingleElemWithId *TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) {
  if (idx >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}

size_t TestStructSingleElemContainer::GetSingleElemStructWithIdCount() const {
  return mSingleElemStructsWithId.size();
}

bool TestStructSingleElemContainer::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestStructSingleElemContainer");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
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

void TestStructSingleElemContainer::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemContainerMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructSingleElemContainer::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "singleElemStructs");
  LuaHelper::Push(mSingleElemStructs, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "singleElemStructsWithId");
  LuaHelper::Push(mSingleElemStructsWithId, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructSingleElemContainer::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructSingleElemContainer *TestStructSingleElemContainer::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElemContainer *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructSingleElemContainer TestStructSingleElemContainer::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructSingleElemContainer{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("singleElemStructs", key)) {
      LuaHelper::Read(result.mSingleElemStructs, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("singleElemStructsWithId", key)) {
      LuaHelper::Read(result.mSingleElemStructsWithId, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructSingleElemContainer::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("singleElemStructs", key)) {
    LuaHelper::Read(instance->mSingleElemStructs, luaState, -1);
  } else if (0 == strcmp("singleElemStructsWithId", key)) {
    LuaHelper::Read(instance->mSingleElemStructsWithId, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
  }
  return 0;
}

void TestStructSingleElemContainer::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructSingleElemContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructSingleElemContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructSingleElemContainerMeta");
}

int TestStructSingleElemContainer::AddSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.AddSingleElemStruct method with an invalid lua proxy object!");
  TestStructSingleElem arg0Mirror;
  TestStructSingleElem *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestStructSingleElem::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestStructSingleElem::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->AddSingleElemStruct(*arg0);
  result->PushToLua(luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStruct method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetSingleElemStruct(arg0);
  result->PushToLua(luaState);
  return 1;
}

int TestStructSingleElemContainer::DeleteSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.DeleteSingleElemStruct method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteSingleElemStruct(arg0);
  return 0;
}

int TestStructSingleElemContainer::GetSingleElemStructCountCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructCount method with an invalid lua proxy object!");
  auto result = instance->GetSingleElemStructCount();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestStructSingleElemContainer::AddSingleElemStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.AddSingleElemStructWithId method with an invalid lua proxy object!");
  auto arg0 = TestStructSingleElemWithId::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddSingleElemStructWithId(*arg0);
  result->PushToLua(luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetSingleElemStructWithId(arg0);
  result->PushToLua(luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructWithIdCountCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructWithIdCount method with an invalid lua proxy object!");
  auto result = instance->GetSingleElemStructWithIdCount();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestStructSingleElemContainer::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("singleElemStructs", key)) {
    auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemContainer.singleElemStructs with an invalid lua proxy object!");
    LuaHelper::Push(instance->mSingleElemStructs, luaState, false);
  } else if (0 == strcmp("singleElemStructsWithId", key)) {
    auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemContainer.singleElemStructsWithId with an invalid lua proxy object!");
    LuaHelper::Push(instance->mSingleElemStructsWithId, luaState, false);
  } else if (0 == strcmp("AddSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::AddSingleElemStructCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructCallerFromLua);
  } else if (0 == strcmp("DeleteSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::DeleteSingleElemStructCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructCount", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructCountCallerFromLua);
  } else if (0 == strcmp("AddSingleElemStructWithId", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::AddSingleElemStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructWithId", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructWithIdCount", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructWithIdCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
    return 0;
  }
  return 1;
}
}
