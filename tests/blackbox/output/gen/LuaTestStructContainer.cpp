// This file is generated by holgen. Do not modify manually.
#include "LuaTestStructContainer.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool LuaTestStructContainer::operator==(const LuaTestStructContainer& rhs) const {
  return
      mTestVector == rhs.mTestVector &&
      mTestMap == rhs.mTestMap;
}

const std::vector<uint32_t>& LuaTestStructContainer::GetTestVector() const {
  return mTestVector;
}

std::vector<uint32_t>& LuaTestStructContainer::GetTestVector() {
  return mTestVector;
}

const std::map<std::string, uint32_t>& LuaTestStructContainer::GetTestMap() const {
  return mTestMap;
}

std::map<std::string, uint32_t>& LuaTestStructContainer::GetTestMap() {
  return mTestMap;
}

void LuaTestStructContainer::SetTestVector(const std::vector<uint32_t>& val) {
  mTestVector = val;
}

void LuaTestStructContainer::SetTestMap(const std::map<std::string, uint32_t>& val) {
  mTestMap = val;
}

uint32_t* LuaTestStructContainer::AddTestVectorElem(uint32_t elem) {
  return &(mTestVector.emplace_back(elem));
}

const uint32_t* LuaTestStructContainer::GetTestVectorElem(size_t idx) const {
  if (idx >= mTestVector.size())
    return nullptr;
  return &mTestVector[idx];
}

uint32_t* LuaTestStructContainer::GetTestVectorElem(size_t idx) {
  if (idx >= mTestVector.size())
    return nullptr;
  return &mTestVector[idx];
}

void LuaTestStructContainer::DeleteTestVectorElem(size_t idx) {
  if (idx != mTestVector.size() - 1) {
    mTestVector[idx] = std::move(mTestVector.back());
  }
  mTestVector.pop_back();
}

size_t LuaTestStructContainer::GetTestVectorElemCount() const {
  return mTestVector.size();
}

bool LuaTestStructContainer::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing LuaTestStructContainer");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("testVector", name)) {
      auto res = JsonHelper::Parse(mTestVector, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse LuaTestStructContainer.testVector field");
    } else if (0 == strcmp("testMap", name)) {
      auto res = JsonHelper::Parse(mTestMap, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse LuaTestStructContainer.testMap field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing LuaTestStructContainer: {}", name);
    }
  }
  return true;
}

void LuaTestStructContainer::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "LuaTestStructContainerMeta");
  lua_setmetatable(luaState, -2);
}

void LuaTestStructContainer::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

LuaTestStructContainer* LuaTestStructContainer::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (LuaTestStructContainer*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int LuaTestStructContainer::IndexMetaMethod(lua_State* luaState) {
  auto instance = LuaTestStructContainer::ReadFromLua(luaState, -2);
  const char* key = lua_tostring(luaState, -1);
  if (0 == strcmp("testVector", key)) {
    LuaHelper::Push(instance->mTestVector, luaState);
  } else if (0 == strcmp("testMap", key)) {
    LuaHelper::Push(instance->mTestMap, luaState);
  } else if (0 == strcmp("GetTestVectorElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = LuaTestStructContainer::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->GetTestVectorElem(arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("DeleteTestVectorElem", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = LuaTestStructContainer::ReadFromLua(lsInner, -2);
      size_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      instance->DeleteTestVectorElem(arg0);
      return 0;
    });
  } else if (0 == strcmp("GetTestVectorElemCount", key)) {
    lua_pushcfunction(luaState, [](lua_State* lsInner) {
      auto instance = LuaTestStructContainer::ReadFromLua(lsInner, -1);
      auto result = instance->GetTestVectorElemCount();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: LuaTestStructContainer.{}", key);
    return 0;
  }
  return 1;
}

int LuaTestStructContainer::NewIndexMetaMethod(lua_State* luaState) {
  auto instance = LuaTestStructContainer::ReadFromLua(luaState, -3);
  const char* key = lua_tostring(luaState, -2);
  if (0 == strcmp("testVector", key)) {
    LuaHelper::Read(instance->mTestVector, luaState, -1);
  } else if (0 == strcmp("testMap", key)) {
    LuaHelper::Read(instance->mTestMap, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: LuaTestStructContainer.{}", key);
  }
  return 0;
}

void LuaTestStructContainer::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, LuaTestStructContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, LuaTestStructContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "LuaTestStructContainerMeta");
}
}
