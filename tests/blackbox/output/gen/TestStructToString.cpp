// This file is generated by holgen. Do not modify manually.
#include "TestStructToString.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestStructToString::operator==(const TestStructToString &rhs) const {
  return !(
      mField1 != rhs.mField1 ||
      mField2 != rhs.mField2
  );
}

const std::string &TestStructToString::GetField1() const {
  return mField1;
}

std::string &TestStructToString::GetField1() {
  return mField1;
}

const std::string &TestStructToString::GetField2() const {
  return mField2;
}

std::string &TestStructToString::GetField2() {
  return mField2;
}

void TestStructToString::SetField1(const std::string &val) {
  mField1 = val;
}

void TestStructToString::SetField2(const std::string &val) {
  mField2 = val;
}

bool TestStructToString::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("field1", name)) {
        auto res = JsonHelper::Parse(mField1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructToString.field1 field");
      } else if (0 == strcmp("field2", name)) {
        auto res = JsonHelper::Parse(mField2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructToString.field2 field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructToString: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructToString!");
      auto res = JsonHelper::Parse(mField1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructToString.field1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructToString!");
      auto res = JsonHelper::Parse(mField2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructToString.field2 field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructToString!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructToString.");
    return false;
  }
  return true;
}

void TestStructToString::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructToStringMeta");
  lua_setmetatable(luaState, -2);
}

void TestStructToString::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "field1");
  LuaHelper::Push(mField1, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "field2");
  LuaHelper::Push(mField2, luaState, true);
  lua_settable(luaState, -3);
}

void TestStructToString::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructToString *TestStructToString::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructToString", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructToString *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructToString TestStructToString::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructToString{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("field1", key)) {
      LuaHelper::Read(result.mField1, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("field2", key)) {
      LuaHelper::Read(result.mField2, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructToString.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructToString::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructToString::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("field1", key)) {
    auto res = LuaHelper::Read(instance->mField1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructToString.field1 from lua failed!");
  } else if (0 == strcmp("field2", key)) {
    auto res = LuaHelper::Read(instance->mField2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructToString.field2 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructToString.{}", key);
  }
  return 0;
}

void TestStructToString::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructToString::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructToString::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructToStringMeta");
}

int TestStructToString::StringifyCallerFromLua(lua_State *luaState) {
  auto instance = TestStructToString::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructToString.Stringify method with an invalid lua proxy object!");
  auto result = instance->Stringify();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestStructToString::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("field1", key)) {
    auto instance = TestStructToString::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructToString.field1 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mField1, luaState, false);
  } else if (0 == strcmp("field2", key)) {
    auto instance = TestStructToString::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructToString.field2 with an invalid lua proxy object!");
    LuaHelper::Push(instance->mField2, luaState, false);
  } else if (0 == strcmp("Stringify", key)) {
    lua_pushcfunction(luaState, TestStructToString::StringifyCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructToString.{}", key);
    return 0;
  }
  return 1;
}
}
