// This file is generated by holgen. Do not modify manually.
#include "TestContainerSet.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerSet::operator==(const TestContainerSet &rhs) const {
  return !(
      mStringContainer != rhs.mStringContainer ||
      mUnsignedContainer != rhs.mUnsignedContainer
  );
}

const std::set<std::string> &TestContainerSet::GetStringContainer() const {
  return mStringContainer;
}

std::set<std::string> &TestContainerSet::GetStringContainer() {
  return mStringContainer;
}

const std::set<uint32_t> &TestContainerSet::GetUnsignedContainer() const {
  return mUnsignedContainer;
}

std::set<uint32_t> &TestContainerSet::GetUnsignedContainer() {
  return mUnsignedContainer;
}

void TestContainerSet::SetStringContainer(const std::set<std::string> &val) {
  mStringContainer = val;
}

void TestContainerSet::SetUnsignedContainer(const std::set<uint32_t> &val) {
  mUnsignedContainer = val;
}

const std::string *TestContainerSet::AddStringElem(std::string &&elem) {
  auto[it, res] = mStringContainer.emplace(std::forward<std::string>(elem));
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Attempting to insert duplicate element to stringContainer");
  return &(*it);
}

const std::string *TestContainerSet::AddStringElem(const std::string &elem) {
  auto[it, res] = mStringContainer.emplace(elem);
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Attempting to insert duplicate element to stringContainer");
  return &(*it);
}

bool TestContainerSet::HasStringElem(const std::string &elem) const {
  return mStringContainer.contains(elem);
}

void TestContainerSet::DeleteStringElem(std::string elem) {
  mStringContainer.erase(elem);
}

size_t TestContainerSet::GetStringElemCount() const {
  return mStringContainer.size();
}

const uint32_t *TestContainerSet::AddUnsignedElem(uint32_t elem) {
  auto[it, res] = mUnsignedContainer.emplace(elem);
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Attempting to insert duplicate element to unsignedContainer");
  return &(*it);
}

bool TestContainerSet::HasUnsignedElem(uint32_t elem) const {
  return mUnsignedContainer.contains(elem);
}

void TestContainerSet::DeleteUnsignedElem(uint32_t elem) {
  mUnsignedContainer.erase(elem);
}

size_t TestContainerSet::GetUnsignedElemCount() const {
  return mUnsignedContainer.size();
}

bool TestContainerSet::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestContainerSet");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("stringContainer", name)) {
      auto res = JsonHelper::Parse(mStringContainer, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerSet.stringContainer field");
    } else if (0 == strcmp("unsignedContainer", name)) {
      auto res = JsonHelper::Parse(mUnsignedContainer, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerSet.unsignedContainer field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestContainerSet: {}", name);
    }
  }
  return true;
}

void TestContainerSet::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerSetMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerSet::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "stringContainer");
  LuaHelper::Push(mStringContainer, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "unsignedContainer");
  LuaHelper::Push(mUnsignedContainer, luaState, true);
  lua_settable(luaState, -3);
}

void TestContainerSet::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerSet *TestContainerSet::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerSet *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerSet TestContainerSet::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerSet{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Read(result.mStringContainer, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Read(result.mUnsignedContainer, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerSet.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerSet::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("stringContainer", key)) {
    LuaHelper::Read(instance->mStringContainer, luaState, -1);
  } else if (0 == strcmp("unsignedContainer", key)) {
    LuaHelper::Read(instance->mUnsignedContainer, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerSet.{}", key);
  }
  return 0;
}

void TestContainerSet::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerSet::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerSet::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerSetMeta");
}

int TestContainerSet::AddStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.AddStringElem method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->AddStringElem(arg0);
  LuaHelper::Push(result, luaState, false);
  return 1;
}

int TestContainerSet::HasStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.HasStringElem method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->HasStringElem(arg0);
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestContainerSet::DeleteStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.DeleteStringElem method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteStringElem(arg0);
  return 0;
}

int TestContainerSet::GetStringElemCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.GetStringElemCount method with an invalid lua proxy object!");
  auto result = instance->GetStringElemCount();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestContainerSet::AddUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.AddUnsignedElem method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->AddUnsignedElem(arg0);
  LuaHelper::Push(result, luaState, false);
  return 1;
}

int TestContainerSet::HasUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.HasUnsignedElem method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->HasUnsignedElem(arg0);
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestContainerSet::DeleteUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.DeleteUnsignedElem method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteUnsignedElem(arg0);
  return 0;
}

int TestContainerSet::GetUnsignedElemCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerSet.GetUnsignedElemCount method with an invalid lua proxy object!");
  auto result = instance->GetUnsignedElemCount();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestContainerSet::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerSet::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerSet.{} with an invalid lua proxy object!", key);
  if (0 == strcmp("stringContainer", key)) {
    LuaHelper::Push(instance->mStringContainer, luaState, false);
  } else if (0 == strcmp("unsignedContainer", key)) {
    LuaHelper::Push(instance->mUnsignedContainer, luaState, false);
  } else if (0 == strcmp("AddStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::AddStringElemCallerFromLua);
  } else if (0 == strcmp("HasStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::HasStringElemCallerFromLua);
  } else if (0 == strcmp("DeleteStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::DeleteStringElemCallerFromLua);
  } else if (0 == strcmp("GetStringElemCount", key)) {
    lua_pushcfunction(luaState, TestContainerSet::GetStringElemCountCallerFromLua);
  } else if (0 == strcmp("AddUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::AddUnsignedElemCallerFromLua);
  } else if (0 == strcmp("HasUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::HasUnsignedElemCallerFromLua);
  } else if (0 == strcmp("DeleteUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerSet::DeleteUnsignedElemCallerFromLua);
  } else if (0 == strcmp("GetUnsignedElemCount", key)) {
    lua_pushcfunction(luaState, TestContainerSet::GetUnsignedElemCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerSet.{}", key);
    return 0;
  }
  return 1;
}
}
