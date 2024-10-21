// This file is generated by holgen. Do not modify manually.
#include "TestJsonStructMapWithConverters.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestJsonStructMapWithConverters::operator==(const TestJsonStructMapWithConverters &rhs) const {
  return !(
      mTestMapConvertElem != rhs.mTestMapConvertElem ||
      mTestMapConvertKey != rhs.mTestMapConvertKey ||
      mTestMapConvertKeyElem != rhs.mTestMapConvertKeyElem
  );
}

const std::map<std::string, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertElem() const {
  return mTestMapConvertElem;
}

std::map<std::string, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertElem() {
  return mTestMapConvertElem;
}

const std::map<uint32_t, std::string> &TestJsonStructMapWithConverters::GetTestMapConvertKey() const {
  return mTestMapConvertKey;
}

std::map<uint32_t, std::string> &TestJsonStructMapWithConverters::GetTestMapConvertKey() {
  return mTestMapConvertKey;
}

const std::map<uint32_t, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertKeyElem() const {
  return mTestMapConvertKeyElem;
}

std::map<uint32_t, uint32_t> &TestJsonStructMapWithConverters::GetTestMapConvertKeyElem() {
  return mTestMapConvertKeyElem;
}

void TestJsonStructMapWithConverters::SetTestMapConvertElem(const std::map<std::string, uint32_t> &val) {
  mTestMapConvertElem = val;
}

void TestJsonStructMapWithConverters::SetTestMapConvertKey(const std::map<uint32_t, std::string> &val) {
  mTestMapConvertKey = val;
}

void TestJsonStructMapWithConverters::SetTestMapConvertKeyElem(const std::map<uint32_t, uint32_t> &val) {
  mTestMapConvertKeyElem = val;
}

bool TestJsonStructMapWithConverters::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("testMapConvertElem", name)) {
        auto res = JsonHelper::ParseConvertElem<std::string>(mTestMapConvertElem, data.value, converter, converter.testJsonConvertStringToU32);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertElem field");
      } else if (0 == strcmp("testMapConvertKey", name)) {
        auto res = JsonHelper::ParseConvertKey<std::string>(mTestMapConvertKey, data.value, converter, converter.testJsonConvertStringToU32);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKey field");
      } else if (0 == strcmp("testMapConvertKeyElem", name)) {
        auto res = JsonHelper::ParseConvertKeyElem<std::string, std::string>(mTestMapConvertKeyElem, data.value, converter, converter.testJsonConvertStringToU32, converter.testJsonConvertStringToU32);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKeyElem field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonStructMapWithConverters: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructMapWithConverters!");
      auto res = JsonHelper::ParseConvertElem<std::string>(mTestMapConvertElem, (*it), converter, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertElem field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructMapWithConverters!");
      auto res = JsonHelper::ParseConvertKey<std::string>(mTestMapConvertKey, (*it), converter, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKey field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructMapWithConverters!");
      auto res = JsonHelper::ParseConvertKeyElem<std::string, std::string>(mTestMapConvertKeyElem, (*it), converter, converter.testJsonConvertStringToU32, converter.testJsonConvertStringToU32);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructMapWithConverters.testMapConvertKeyElem field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestJsonStructMapWithConverters!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestJsonStructMapWithConverters.");
    return false;
  }
  return true;
}

void TestJsonStructMapWithConverters::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonStructMapWithConvertersMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonStructMapWithConverters::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "testMapConvertElem");
  LuaHelper::Push<true>(mTestMapConvertElem, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "testMapConvertKey");
  LuaHelper::Push<true>(mTestMapConvertKey, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "testMapConvertKeyElem");
  LuaHelper::Push<true>(mTestMapConvertKeyElem, luaState);
  lua_settable(luaState, -3);
}

void TestJsonStructMapWithConverters::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonStructMapWithConverters *TestJsonStructMapWithConverters::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestJsonStructMapWithConverters", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonStructMapWithConverters *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonStructMapWithConverters TestJsonStructMapWithConverters::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonStructMapWithConverters{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("testMapConvertElem", key)) {
      LuaHelper::Read(result.mTestMapConvertElem, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("testMapConvertKey", key)) {
      LuaHelper::Read(result.mTestMapConvertKey, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("testMapConvertKeyElem", key)) {
      LuaHelper::Read(result.mTestMapConvertKeyElem, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonStructMapWithConverters.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonStructMapWithConverters::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructMapWithConverters::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("testMapConvertElem", key)) {
    auto res = LuaHelper::Read(instance->mTestMapConvertElem, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructMapWithConverters.testMapConvertElem from lua failed!");
  } else if (0 == strcmp("testMapConvertKey", key)) {
    auto res = LuaHelper::Read(instance->mTestMapConvertKey, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructMapWithConverters.testMapConvertKey from lua failed!");
  } else if (0 == strcmp("testMapConvertKeyElem", key)) {
    auto res = LuaHelper::Read(instance->mTestMapConvertKeyElem, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructMapWithConverters.testMapConvertKeyElem from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructMapWithConverters.{}", key);
  }
  return 0;
}

void TestJsonStructMapWithConverters::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonStructMapWithConverters::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonStructMapWithConverters::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonStructMapWithConvertersMeta");
}

int TestJsonStructMapWithConverters::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("testMapConvertElem", key)) {
    auto instance = TestJsonStructMapWithConverters::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructMapWithConverters.testMapConvertElem with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTestMapConvertElem, luaState);
  } else if (0 == strcmp("testMapConvertKey", key)) {
    auto instance = TestJsonStructMapWithConverters::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructMapWithConverters.testMapConvertKey with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTestMapConvertKey, luaState);
  } else if (0 == strcmp("testMapConvertKeyElem", key)) {
    auto instance = TestJsonStructMapWithConverters::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructMapWithConverters.testMapConvertKeyElem with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTestMapConvertKeyElem, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructMapWithConverters.{}", key);
    return 0;
  }
  return 1;
}
}
