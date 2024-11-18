// This file is generated by holgen. Do not modify manually.
#include "TestStructSmartPointers.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestStructSmartPointers::TestStructSmartPointers(TestStructSmartPointers &&rhs) noexcept {
  mSharedPtr = std::move(rhs.mSharedPtr);
  mSharedPtrs = std::move(rhs.mSharedPtrs);
  mUniquePtr = std::move(rhs.mUniquePtr);
  mUniquePtrs = std::move(rhs.mUniquePtrs);
}

const std::shared_ptr<TestStructVirtualMethods> &TestStructSmartPointers::GetSharedPtr() const {
  return mSharedPtr;
}

std::shared_ptr<TestStructVirtualMethods> &TestStructSmartPointers::GetSharedPtr() {
  return mSharedPtr;
}

const std::vector<std::shared_ptr<TestStructVirtualMethods>> &TestStructSmartPointers::GetSharedPtrs() const {
  return mSharedPtrs;
}

std::vector<std::shared_ptr<TestStructVirtualMethods>> &TestStructSmartPointers::GetSharedPtrs() {
  return mSharedPtrs;
}

const std::unique_ptr<TestStructVirtualMethods> &TestStructSmartPointers::GetUniquePtr() const {
  return mUniquePtr;
}

std::unique_ptr<TestStructVirtualMethods> &TestStructSmartPointers::GetUniquePtr() {
  return mUniquePtr;
}

const std::vector<std::unique_ptr<TestStructVirtualMethods>> &TestStructSmartPointers::GetUniquePtrs() const {
  return mUniquePtrs;
}

std::vector<std::unique_ptr<TestStructVirtualMethods>> &TestStructSmartPointers::GetUniquePtrs() {
  return mUniquePtrs;
}

void TestStructSmartPointers::SetSharedPtr(std::shared_ptr<TestStructVirtualMethods> &&val) {
  mSharedPtr = std::move(val);
}

void TestStructSmartPointers::SetSharedPtrs(std::vector<std::shared_ptr<TestStructVirtualMethods>> &&val) {
  mSharedPtrs = std::move(val);
}

void TestStructSmartPointers::SetUniquePtr(std::unique_ptr<TestStructVirtualMethods> &&val) {
  mUniquePtr = std::move(val);
}

void TestStructSmartPointers::SetUniquePtrs(std::vector<std::unique_ptr<TestStructVirtualMethods>> &&val) {
  mUniquePtrs = std::move(val);
}

bool TestStructSmartPointers::operator==(const TestStructSmartPointers &rhs) const {
  return !(
      mSharedPtr != rhs.mSharedPtr ||
      mSharedPtrs != rhs.mSharedPtrs ||
      mUniquePtr != rhs.mUniquePtr ||
      mUniquePtrs != rhs.mUniquePtrs
  );
}

bool TestStructSmartPointers::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  return true;
}

rapidjson::Value TestStructSmartPointers::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  return val;
}

void TestStructSmartPointers::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSmartPointers");
  lua_setmetatable(luaState, -2);
}

void TestStructSmartPointers::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructSmartPointers *TestStructSmartPointers::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructSmartPointers", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSmartPointers *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestStructSmartPointers::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestStructSmartPointers::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructSmartPointers::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructSmartPointers::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructSmartPointers");
}

int TestStructSmartPointers::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("sharedPtr", key)) {
    auto instance = TestStructSmartPointers::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSmartPointers.sharedPtr with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mSharedPtr, luaState);
  } else if (0 == strcmp("sharedPtrs", key)) {
    auto instance = TestStructSmartPointers::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSmartPointers.sharedPtrs with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mSharedPtrs, luaState);
  } else if (0 == strcmp("uniquePtr", key)) {
    auto instance = TestStructSmartPointers::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSmartPointers.uniquePtr with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mUniquePtr, luaState);
  } else if (0 == strcmp("uniquePtrs", key)) {
    auto instance = TestStructSmartPointers::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSmartPointers.uniquePtrs with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mUniquePtrs, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSmartPointers.{}", key);
    return 0;
  }
  return 1;
}

TestStructSmartPointers &TestStructSmartPointers::operator=(TestStructSmartPointers &&rhs) noexcept {
  mSharedPtr = std::move(rhs.mSharedPtr);
  mSharedPtrs = std::move(rhs.mSharedPtrs);
  mUniquePtr = std::move(rhs.mUniquePtr);
  mUniquePtrs = std::move(rhs.mUniquePtrs);
  return *this;
}
}
