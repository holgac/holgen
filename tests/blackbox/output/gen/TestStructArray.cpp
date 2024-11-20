// This file is generated by holgen. Do not modify manually.
#include "TestStructArray.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestStructArrayCustomData1.h"
#include "TestStructArrayCustomData2.h"

namespace holgen_blackbox_test {
const TestStructArrayType &TestStructArray::GetType() const {
  return mType;
}

TestStructArrayType &TestStructArray::GetType() {
  return mType;
}

const std::array<uint8_t, 16> &TestStructArray::GetCustomData() const {
  return mCustomData;
}

std::array<uint8_t, 16> &TestStructArray::GetCustomData() {
  return mCustomData;
}

void TestStructArray::SetType(const TestStructArrayType &val) {
  mType = val;
}

void TestStructArray::SetCustomData(const std::array<uint8_t, 16> &val) {
  mCustomData = val;
}

bool TestStructArray::operator==(const TestStructArray &rhs) const {
  return !(
      mType != rhs.mType ||
      mCustomData != rhs.mCustomData
  );
}

bool TestStructArray::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("type", name)) {
        auto res = JsonHelper::Parse(mType, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArray.type field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructArray: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArray!");
      auto res = JsonHelper::Parse(mType, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArray.type field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructArray!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructArray.");
    return false;
  }
  return true;
}

rapidjson::Value TestStructArray::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("type", JsonHelper::Dump(mType, doc, luaState), doc.GetAllocator());
  return val;
}

void TestStructArray::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructArray");
  lua_setmetatable(luaState, -2);
}

void TestStructArray::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "type");
  LuaHelper::Push<true>(mType, luaState);
  lua_settable(luaState, -3);
}

void TestStructArray::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructArray *TestStructArray::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructArray", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructArray *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructArray TestStructArray::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructArray{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("type", key)) {
      LuaHelper::Read(result.mType, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructArray::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructArray::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    auto res = LuaHelper::Read(instance->mType, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArray.type from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
  }
  return 0;
}

void TestStructArray::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructArray::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructArray::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructArray");
}

int TestStructArray::GetData1CallerFromLua(lua_State *luaState) {
  auto instance = TestStructArray::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructArray.GetData1 method with an invalid lua proxy object!");
  auto result = instance->GetData1();
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructArray::GetData2CallerFromLua(lua_State *luaState) {
  auto instance = TestStructArray::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructArray.GetData2 method with an invalid lua proxy object!");
  auto result = instance->GetData2();
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructArray::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    auto instance = TestStructArray::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArray.type with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mType, luaState);
  } else if (0 == strcmp("GetData1", key)) {
    lua_pushcfunction(luaState, TestStructArray::GetData1CallerFromLua);
  } else if (0 == strcmp("GetData2", key)) {
    lua_pushcfunction(luaState, TestStructArray::GetData2CallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArray.{}", key);
    return 0;
  }
  return 1;
}
}
