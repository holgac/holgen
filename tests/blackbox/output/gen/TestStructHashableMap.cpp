// This file is generated by holgen. Do not modify manually.
#include "TestStructHashableMap.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::unordered_map<TestStructHashable, std::string> &TestStructHashableMap::GetData() const {
  return mData;
}

std::unordered_map<TestStructHashable, std::string> &TestStructHashableMap::GetData() {
  return mData;
}

void TestStructHashableMap::SetData(const std::unordered_map<TestStructHashable, std::string> &val) {
  mData = val;
}

bool TestStructHashableMap::operator==(const TestStructHashableMap &rhs) const {
  return !(
      mData != rhs.mData
  );
}

bool TestStructHashableMap::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("data", name)) {
        auto res = JsonHelper::Parse(mData, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashableMap.data field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructHashableMap: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructHashableMap!");
      auto res = JsonHelper::Parse(mData, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructHashableMap.data field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructHashableMap!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructHashableMap.");
    return false;
  }
  return true;
}

rapidjson::Value TestStructHashableMap::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("data", JsonHelper::Dump(mData, doc, luaState), doc.GetAllocator());
  return val;
}

void TestStructHashableMap::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructHashableMap");
  lua_setmetatable(luaState, -2);
}

void TestStructHashableMap::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "data");
  LuaHelper::Push<true>(mData, luaState);
  lua_settable(luaState, -3);
}

void TestStructHashableMap::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructHashableMap *TestStructHashableMap::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructHashableMap", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructHashableMap *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructHashableMap TestStructHashableMap::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructHashableMap{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("data", key)) {
      LuaHelper::Read(result.mData, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructHashableMap.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructHashableMap::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructHashableMap::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("data", key)) {
    auto res = LuaHelper::Read(instance->mData, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructHashableMap.data from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructHashableMap.{}", key);
  }
  return 0;
}

void TestStructHashableMap::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructHashableMap::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructHashableMap::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructHashableMap");
}

int TestStructHashableMap::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("data", key)) {
    auto instance = TestStructHashableMap::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructHashableMap.data with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mData, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructHashableMap.{}", key);
    return 0;
  }
  return 1;
}
}
