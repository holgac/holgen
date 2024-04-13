// This file is generated by holgen. Do not modify manually.
#include "TestContainerVector.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerVector::operator==(const TestContainerVector& rhs) const {
  return
      mInnerStructsWithId == rhs.mInnerStructsWithId &&
      mInnerStructsNoId == rhs.mInnerStructsNoId &&
      mStringContainer == rhs.mStringContainer &&
      mUnsignedContainer == rhs.mUnsignedContainer;
}

const std::vector<TestContainerInnerStructWithId>& TestContainerVector::GetInnerStructsWithId() const {
  return mInnerStructsWithId;
}

std::vector<TestContainerInnerStructWithId>& TestContainerVector::GetInnerStructsWithId() {
  return mInnerStructsWithId;
}

const std::vector<TestContainerInnerStructNoId>& TestContainerVector::GetInnerStructsNoId() const {
  return mInnerStructsNoId;
}

std::vector<TestContainerInnerStructNoId>& TestContainerVector::GetInnerStructsNoId() {
  return mInnerStructsNoId;
}

const std::vector<std::string>& TestContainerVector::GetStringContainer() const {
  return mStringContainer;
}

std::vector<std::string>& TestContainerVector::GetStringContainer() {
  return mStringContainer;
}

const std::vector<uint32_t>& TestContainerVector::GetUnsignedContainer() const {
  return mUnsignedContainer;
}

std::vector<uint32_t>& TestContainerVector::GetUnsignedContainer() {
  return mUnsignedContainer;
}

void TestContainerVector::SetInnerStructsWithId(const std::vector<TestContainerInnerStructWithId>& val) {
  mInnerStructsWithId = val;
}

void TestContainerVector::SetInnerStructsNoId(const std::vector<TestContainerInnerStructNoId>& val) {
  mInnerStructsNoId = val;
}

void TestContainerVector::SetStringContainer(const std::vector<std::string>& val) {
  mStringContainer = val;
}

void TestContainerVector::SetUnsignedContainer(const std::vector<uint32_t>& val) {
  mUnsignedContainer = val;
}

const TestContainerInnerStructWithId* TestContainerVector::GetInnerStructWithIdFromName(const std::string& key) const {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId[it->second];
}

TestContainerInnerStructWithId* TestContainerVector::GetInnerStructWithIdFromName(const std::string& key) {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId[it->second];
}

TestContainerInnerStructWithId* TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId&& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsWithId.size();
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mInnerStructsWithId.emplace_back(std::forward<TestContainerInnerStructWithId>(elem)));
}

TestContainerInnerStructWithId* TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsWithId.size();
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  return &(mInnerStructsWithId.emplace_back(elem));
}

const TestContainerInnerStructWithId* TestContainerVector::GetInnerStructWithId(uint32_t idx) const {
  if (idx >= mInnerStructsWithId.size())
    return nullptr;
  return &mInnerStructsWithId[idx];
}

TestContainerInnerStructWithId* TestContainerVector::GetInnerStructWithId(uint32_t idx) {
  if (idx >= mInnerStructsWithId.size())
    return nullptr;
  return &mInnerStructsWithId[idx];
}

size_t TestContainerVector::GetInnerStructWithIdCount() const {
  return mInnerStructsWithId.size();
}

const TestContainerInnerStructNoId* TestContainerVector::GetInnerStructNoIdFromName(const std::string& key) const {
  auto it = mInnerStructsNoIdNameIndex.find(key);
  if (it == mInnerStructsNoIdNameIndex.end())
    return nullptr;
  return &mInnerStructsNoId[it->second];
}

TestContainerInnerStructNoId* TestContainerVector::GetInnerStructNoIdFromName(const std::string& key) {
  auto it = mInnerStructsNoIdNameIndex.find(key);
  if (it == mInnerStructsNoIdNameIndex.end())
    return nullptr;
  return &mInnerStructsNoId[it->second];
}

TestContainerInnerStructNoId* TestContainerVector::AddInnerStructNoId(TestContainerInnerStructNoId&& elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructNoId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  return &(mInnerStructsNoId.emplace_back(std::forward<TestContainerInnerStructNoId>(elem)));
}

TestContainerInnerStructNoId* TestContainerVector::AddInnerStructNoId(const TestContainerInnerStructNoId& elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructNoId with name={} already exists", elem.GetName());
    return nullptr;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  return &(mInnerStructsNoId.emplace_back(elem));
}

const TestContainerInnerStructNoId* TestContainerVector::GetInnerStructNoId(size_t idx) const {
  if (idx >= mInnerStructsNoId.size())
    return nullptr;
  return &mInnerStructsNoId[idx];
}

TestContainerInnerStructNoId* TestContainerVector::GetInnerStructNoId(size_t idx) {
  if (idx >= mInnerStructsNoId.size())
    return nullptr;
  return &mInnerStructsNoId[idx];
}

void TestContainerVector::DeleteInnerStructNoId(size_t idx) {
  auto ptr = GetInnerStructNoId(idx);
  mInnerStructsNoIdNameIndex.erase(ptr->GetName());
  if (idx != mInnerStructsNoId.size() - 1) {
    mInnerStructsNoIdNameIndex.at(mInnerStructsNoId.back().GetName()) = idx;
    mInnerStructsNoId[idx] = std::move(mInnerStructsNoId.back());
  }
  mInnerStructsNoId.pop_back();
}

size_t TestContainerVector::GetInnerStructNoIdCount() const {
  return mInnerStructsNoId.size();
}

std::string* TestContainerVector::AddStringElem(std::string&& elem) {
  return &(mStringContainer.emplace_back(std::forward<std::string>(elem)));
}

std::string* TestContainerVector::AddStringElem(const std::string& elem) {
  return &(mStringContainer.emplace_back(elem));
}

const std::string* TestContainerVector::GetStringElem(size_t idx) const {
  if (idx >= mStringContainer.size())
    return nullptr;
  return &mStringContainer[idx];
}

std::string* TestContainerVector::GetStringElem(size_t idx) {
  if (idx >= mStringContainer.size())
    return nullptr;
  return &mStringContainer[idx];
}

void TestContainerVector::DeleteStringElem(size_t idx) {
  if (idx != mStringContainer.size() - 1) {
    mStringContainer[idx] = std::move(mStringContainer.back());
  }
  mStringContainer.pop_back();
}

size_t TestContainerVector::GetStringElemCount() const {
  return mStringContainer.size();
}

uint32_t* TestContainerVector::AddUnsignedElem(uint32_t elem) {
  return &(mUnsignedContainer.emplace_back(elem));
}

const uint32_t* TestContainerVector::GetUnsignedElem(size_t idx) const {
  if (idx >= mUnsignedContainer.size())
    return nullptr;
  return &mUnsignedContainer[idx];
}

uint32_t* TestContainerVector::GetUnsignedElem(size_t idx) {
  if (idx >= mUnsignedContainer.size())
    return nullptr;
  return &mUnsignedContainer[idx];
}

void TestContainerVector::DeleteUnsignedElem(size_t idx) {
  if (idx != mUnsignedContainer.size() - 1) {
    mUnsignedContainer[idx] = std::move(mUnsignedContainer.back());
  }
  mUnsignedContainer.pop_back();
}

size_t TestContainerVector::GetUnsignedElemCount() const {
  return mUnsignedContainer.size();
}

bool TestContainerVector::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestContainerVector");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("innerStructsWithId", name)) {
      auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsWithId field");
    } else if (0 == strcmp("innerStructsNoId", name)) {
      auto res = JsonHelper::Parse(mInnerStructsNoId, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsNoId field");
    } else if (0 == strcmp("stringContainer", name)) {
      auto res = JsonHelper::Parse(mStringContainer, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.stringContainer field");
    } else if (0 == strcmp("unsignedContainer", name)) {
      auto res = JsonHelper::Parse(mUnsignedContainer, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.unsignedContainer field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestContainerVector: {}", name);
    }
  }
  return true;
}

void TestContainerVector::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerVectorMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerVector::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerVector* TestContainerVector::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerVector*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestContainerVector::IndexMetaMethod(lua_State* luaState) {
  auto instance = TestContainerVector::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("innerStructsWithId", key)) {
    LuaHelper::Push(instance->mInnerStructsWithId, luaState);
  } else if (0 == strcmp("innerStructsNoId", key)) {
    LuaHelper::Push(instance->mInnerStructsNoId, luaState);
  } else if (0 == strcmp("stringContainer", key)) {
    LuaHelper::Push(instance->mStringContainer, luaState);
  } else if (0 == strcmp("unsignedContainer", key)) {
    LuaHelper::Push(instance->mUnsignedContainer, luaState);
  } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      std::string arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructWithIdFromName(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetInnerStructWithId", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      uint32_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructWithId(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
      auto result = instance->GetInnerStructWithIdCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetInnerStructNoIdFromName", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      std::string arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructNoIdFromName(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetInnerStructNoId", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetInnerStructNoId(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("DeleteInnerStructNoId", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      instance->DeleteInnerStructNoId(arg0);
      return 0;
    });
  } else if (0 == strcmp("GetInnerStructNoIdCount", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
      auto result = instance->GetInnerStructNoIdCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetStringElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetStringElem(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("DeleteStringElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      instance->DeleteStringElem(arg0);
      return 0;
    });
  } else if (0 == strcmp("GetStringElemCount", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
      auto result = instance->GetStringElemCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("GetUnsignedElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetUnsignedElem(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("DeleteUnsignedElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      instance->DeleteUnsignedElem(arg0);
      return 0;
    });
  } else if (0 == strcmp("GetUnsignedElemCount", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
      auto result = instance->GetUnsignedElemCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerVector.{}", key);
    return 0;
  }
  return 1;
}

int TestContainerVector::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = TestContainerVector::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("innerStructsWithId", key)) {
    LuaHelper::Read(instance->mInnerStructsWithId, luaState, -1);
  } else if (0 == strcmp("innerStructsNoId", key)) {
    LuaHelper::Read(instance->mInnerStructsNoId, luaState, -1);
  } else if (0 == strcmp("stringContainer", key)) {
    LuaHelper::Read(instance->mStringContainer, luaState, -1);
  } else if (0 == strcmp("unsignedContainer", key)) {
    LuaHelper::Read(instance->mUnsignedContainer, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerVector.{}", key);
  }
  return 0;
}

void TestContainerVector::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerVector::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerVector::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerVectorMeta");
}
}
