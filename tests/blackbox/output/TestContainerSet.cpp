#include "TestContainerSet.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::set<std::string>& TestContainerSet::GetStringContainer() const {
  return mStringContainer;
}
std::set<std::string>& TestContainerSet::GetStringContainer() {
  return mStringContainer;
}
const std::set<uint32_t>& TestContainerSet::GetUnsignedContainer() const {
  return mUnsignedContainer;
}
std::set<uint32_t>& TestContainerSet::GetUnsignedContainer() {
  return mUnsignedContainer;
}
void TestContainerSet::SetStringContainer(const std::set<std::string>& val) {
  mStringContainer = val;
}
void TestContainerSet::SetUnsignedContainer(const std::set<uint32_t>& val) {
  mUnsignedContainer = val;
}
bool TestContainerSet::AddStringElem(std::string&& elem) {
  mStringContainer.emplace(std::forward<std::string>(elem));
  return true;
}
bool TestContainerSet::AddStringElem(const std::string& elem) {
  mStringContainer.emplace(elem);
  return true;
}
bool TestContainerSet::HasStringElem(const std::string& elem) const {
  return mStringContainer.contains(elem);
}
void TestContainerSet::DeleteStringElem(std::string elem) {
  mStringContainer.erase(elem);
}
size_t TestContainerSet::GetStringElemCount() const {
  return mStringContainer.size();
}
bool TestContainerSet::AddUnsignedElem(uint32_t&& elem) {
  mUnsignedContainer.emplace(std::forward<uint32_t>(elem));
  return true;
}
bool TestContainerSet::AddUnsignedElem(const uint32_t& elem) {
  mUnsignedContainer.emplace(elem);
  return true;
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
bool TestContainerSet::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "stringContainer")) {
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
void TestContainerSet::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerSetMeta");
  lua_setmetatable(luaState, -2);
}
void TestContainerSet::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}
TestContainerSet* TestContainerSet::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerSet*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void TestContainerSet::PushIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerSet::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Push(instance->mStringContainer, ls);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Push(instance->mUnsignedContainer, ls);
    } else if (0 == strcmp("HasStringElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->HasStringElem(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteStringElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -2);
        std::string arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteStringElem(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetStringElemCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -1);
        auto result = instance->GetStringElemCount();
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("HasUnsignedElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        auto result = instance->HasUnsignedElem(arg0);
        LuaHelper::Push(result, lsInner);
        return 1;
      });
    } else if (0 == strcmp("DeleteUnsignedElem", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -2);
        uint32_t arg0;
        LuaHelper::Read(arg0, lsInner, -1);
        instance->DeleteUnsignedElem(arg0);
        return 0;
      });
    } else if (0 == strcmp("GetUnsignedElemCount", key)) {
      lua_pushcfunction(ls, [](lua_State* lsInner) {
        auto instance = TestContainerSet::ReadFromLua(lsInner, -1);
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
void TestContainerSet::PushNewIndexMetaMethod(lua_State* luaState) {
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = TestContainerSet::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Read(instance->mStringContainer, ls, -1);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Read(instance->mUnsignedContainer, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
}
void TestContainerSet::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  PushIndexMetaMethod(luaState);
  PushNewIndexMetaMethod(luaState);
  lua_setglobal(luaState, "TestContainerSetMeta");
}
}
