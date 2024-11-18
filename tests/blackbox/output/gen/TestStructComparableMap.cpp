// This file is generated by holgen. Do not modify manually.
#include "TestStructComparableMap.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::map<TestStructComparable, std::string> &TestStructComparableMap::GetData() const {
  return mData;
}

std::map<TestStructComparable, std::string> &TestStructComparableMap::GetData() {
  return mData;
}

void TestStructComparableMap::SetData(const std::map<TestStructComparable, std::string> &val) {
  mData = val;
}

bool TestStructComparableMap::operator==(const TestStructComparableMap &rhs) const {
  return !(
      mData != rhs.mData
  );
}

bool TestStructComparableMap::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("data", name)) {
        auto res = JsonHelper::Parse(mData, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructComparableMap.data field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructComparableMap: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructComparableMap!");
      auto res = JsonHelper::Parse(mData, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructComparableMap.data field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructComparableMap!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructComparableMap.");
    return false;
  }
  return true;
}

rapidjson::Value TestStructComparableMap::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("data", JsonHelper::Dump(mData, doc), doc.GetAllocator());
  return val;
}

void TestStructComparableMap::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructComparableMap");
  lua_setmetatable(luaState, -2);
}

void TestStructComparableMap::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "data");
  LuaHelper::Push<true>(mData, luaState);
  lua_settable(luaState, -3);
}

void TestStructComparableMap::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructComparableMap *TestStructComparableMap::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructComparableMap", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructComparableMap *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructComparableMap TestStructComparableMap::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructComparableMap{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("data", key)) {
      LuaHelper::Read(result.mData, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructComparableMap.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructComparableMap::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructComparableMap::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("data", key)) {
    auto res = LuaHelper::Read(instance->mData, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructComparableMap.data from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructComparableMap.{}", key);
  }
  return 0;
}

void TestStructComparableMap::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructComparableMap::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructComparableMap::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructComparableMap");
}

int TestStructComparableMap::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("data", key)) {
    auto instance = TestStructComparableMap::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructComparableMap.data with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mData, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructComparableMap.{}", key);
    return 0;
  }
  return 1;
}
}
