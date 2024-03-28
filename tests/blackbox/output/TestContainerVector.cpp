#include "TestContainerVector.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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
bool TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId&& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsWithId.size();
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mInnerStructsWithId.emplace_back(std::forward<TestContainerInnerStructWithId>(elem));
  return true;
}
bool TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId& elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructWithId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsWithId.size();
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  elem.SetId(newId);
  mInnerStructsWithId.emplace_back(elem);
  return true;
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
bool TestContainerVector::AddInnerStructNoId(TestContainerInnerStructNoId&& elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructNoId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  mInnerStructsNoId.emplace_back(std::forward<TestContainerInnerStructNoId>(elem));
  return true;
}
bool TestContainerVector::AddInnerStructNoId(const TestContainerInnerStructNoId& elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    HOLGEN_WARN("TestContainerInnerStructNoId with name={} already exists!", elem.GetName());
    return false;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  mInnerStructsNoId.emplace_back(elem);
  return true;
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
bool TestContainerVector::AddStringElem(std::string&& elem) {
  mStringContainer.emplace_back(std::forward<std::string>(elem));
  return true;
}
bool TestContainerVector::AddStringElem(const std::string& elem) {
  mStringContainer.emplace_back(elem);
  return true;
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
bool TestContainerVector::AddUnsignedElem(uint32_t&& elem) {
  mUnsignedContainer.emplace_back(std::forward<uint32_t>(elem));
  return true;
}
bool TestContainerVector::AddUnsignedElem(const uint32_t& elem) {
  mUnsignedContainer.emplace_back(elem);
  return true;
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
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "innerStructsWithId")) {
      auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "innerStructsNoId")) {
      auto res = JsonHelper::Parse(mInnerStructsNoId, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "stringContainer")) {
      auto res = JsonHelper::Parse(mStringContainer, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "unsignedContainer")) {
      auto res = JsonHelper::Parse(mUnsignedContainer, data.value, converter);
      if (!res)
        return false;
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
void TestContainerVector::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerVector::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Push(instance->mInnerStructsWithId, ls);
    } else if (0 == strcmp("innerStructsNoId", key)) {
      LuaHelper::Push(instance->mInnerStructsNoId, ls);
    } else if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Push(instance->mStringContainer, ls);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Push(instance->mUnsignedContainer, ls);
    } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructWithIdFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetInnerStructWithId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructWithId(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
        auto result = instance->GetInnerStructWithIdCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetInnerStructNoIdFromName", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructNoIdFromName(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetInnerStructNoId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetInnerStructNoId(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteInnerStructNoId", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteInnerStructNoId(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetInnerStructNoIdCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
        auto result = instance->GetInnerStructNoIdCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetStringElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetStringElem(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteStringElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteStringElem(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetStringElemCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
        auto result = instance->GetStringElemCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("GetUnsignedElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->GetUnsignedElem(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteUnsignedElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -2);
        size_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteUnsignedElem(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetUnsignedElemCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerVector::ReadFromLua(lsInner, -1);
        auto result = instance->GetUnsignedElemCount();
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
void TestContainerVector::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerVector::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Read(instance->mInnerStructsWithId, ls, -1);
    } else if (0 == strcmp("innerStructsNoId", key)) {
      LuaHelper::Read(instance->mInnerStructsNoId, ls, -1);
    } else if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Read(instance->mStringContainer, ls, -1);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Read(instance->mUnsignedContainer, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestContainerVector::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestContainerVectorMeta");
}
}