// This file is generated by holgen. Do not modify manually.
#include "TestEnumStruct.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestEnumStruct::operator==(const TestEnumStruct &rhs) const {
  return !(
      mEnumField != rhs.mEnumField ||
      mEnumDefaultValueField != rhs.mEnumDefaultValueField
  );
}

const TestEnum &TestEnumStruct::GetEnumField() const {
  return mEnumField;
}

TestEnum &TestEnumStruct::GetEnumField() {
  return mEnumField;
}

const TestEnumDefaultValue &TestEnumStruct::GetEnumDefaultValueField() const {
  return mEnumDefaultValueField;
}

TestEnumDefaultValue &TestEnumStruct::GetEnumDefaultValueField() {
  return mEnumDefaultValueField;
}

void TestEnumStruct::SetEnumField(const TestEnum &val) {
  mEnumField = val;
}

void TestEnumStruct::SetEnumDefaultValueField(const TestEnumDefaultValue &val) {
  mEnumDefaultValueField = val;
}

bool TestEnumStruct::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("enumField", name)) {
        auto res = JsonHelper::Parse(mEnumField, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestEnumStruct.enumField field");
      } else if (0 == strcmp("enumDefaultValueField", name)) {
        auto res = JsonHelper::Parse(mEnumDefaultValueField, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestEnumStruct.enumDefaultValueField field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestEnumStruct: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestEnumStruct!");
      auto res = JsonHelper::Parse(mEnumField, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestEnumStruct.enumField field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestEnumStruct!");
      auto res = JsonHelper::Parse(mEnumDefaultValueField, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestEnumStruct.enumDefaultValueField field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestEnumStruct!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestEnumStruct.");
    return false;
  }
  return true;
}

void TestEnumStruct::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestEnumStructMeta");
  lua_setmetatable(luaState, -2);
}

void TestEnumStruct::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "enumField");
  LuaHelper::Push(mEnumField, luaState, true);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "enumDefaultValueField");
  LuaHelper::Push(mEnumDefaultValueField, luaState, true);
  lua_settable(luaState, -3);
}

void TestEnumStruct::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestEnumStruct *TestEnumStruct::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestEnumStruct", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestEnumStruct *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestEnumStruct TestEnumStruct::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestEnumStruct{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("enumField", key)) {
      LuaHelper::Read(result.mEnumField, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("enumDefaultValueField", key)) {
      LuaHelper::Read(result.mEnumDefaultValueField, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestEnumStruct.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestEnumStruct::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestEnumStruct::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("enumField", key)) {
    auto res = LuaHelper::Read(instance->mEnumField, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestEnumStruct.enumField from lua failed!");
  } else if (0 == strcmp("enumDefaultValueField", key)) {
    auto res = LuaHelper::Read(instance->mEnumDefaultValueField, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestEnumStruct.enumDefaultValueField from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEnumStruct.{}", key);
  }
  return 0;
}

void TestEnumStruct::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestEnumStruct::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestEnumStruct::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestEnumStructMeta");
}

int TestEnumStruct::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("enumField", key)) {
    auto instance = TestEnumStruct::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestEnumStruct.enumField with an invalid lua proxy object!");
    LuaHelper::Push(instance->mEnumField, luaState, false);
  } else if (0 == strcmp("enumDefaultValueField", key)) {
    auto instance = TestEnumStruct::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestEnumStruct.enumDefaultValueField with an invalid lua proxy object!");
    LuaHelper::Push(instance->mEnumDefaultValueField, luaState, false);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestEnumStruct.{}", key);
    return 0;
  }
  return 1;
}
}
