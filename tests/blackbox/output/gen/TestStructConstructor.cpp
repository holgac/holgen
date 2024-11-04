// This file is generated by holgen. Do not modify manually.
#include "TestStructConstructor.h"

#include <cmath>
#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
float TestStructConstructor::GetX() const {
  return mX;
}

float TestStructConstructor::GetY() const {
  return mY;
}

float TestStructConstructor::GetZ() const {
  return mZ;
}

void TestStructConstructor::SetX(float val) {
  mX = val;
}

void TestStructConstructor::SetY(float val) {
  mY = val;
}

void TestStructConstructor::SetZ(float val) {
  mZ = val;
}

TestStructConstructor TestStructConstructor::Construct1(const float val) {
  return TestStructConstructor(val);
}

TestStructConstructor TestStructConstructor::Construct3(const float x, const float y, const float z) {
  return TestStructConstructor(x, y, z);
}

bool TestStructConstructor::operator==(const TestStructConstructor &rhs) const {
  return !(
      std::fabs(mX - rhs.mX) >= 0.00001 ||
      std::fabs(mY - rhs.mY) >= 0.00001 ||
      std::fabs(mZ - rhs.mZ) >= 0.00001
  );
}

bool TestStructConstructor::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("x", name)) {
        auto res = JsonHelper::Parse(mX, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.x field");
      } else if (0 == strcmp("y", name)) {
        auto res = JsonHelper::Parse(mY, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.y field");
      } else if (0 == strcmp("z", name)) {
        auto res = JsonHelper::Parse(mZ, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.z field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructConstructor: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructConstructor!");
      auto res = JsonHelper::Parse(mX, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.x field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructConstructor!");
      auto res = JsonHelper::Parse(mY, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.y field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructConstructor!");
      auto res = JsonHelper::Parse(mZ, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructConstructor.z field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructConstructor!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructConstructor.");
    return false;
  }
  return true;
}

void TestStructConstructor::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructConstructor");
  lua_setmetatable(luaState, -2);
}

void TestStructConstructor::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "x");
  LuaHelper::Push<true>(mX, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "y");
  LuaHelper::Push<true>(mY, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "z");
  LuaHelper::Push<true>(mZ, luaState);
  lua_settable(luaState, -3);
}

void TestStructConstructor::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructConstructor *TestStructConstructor::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructConstructor", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructConstructor *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructConstructor TestStructConstructor::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructConstructor{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("x", key)) {
      LuaHelper::Read(result.mX, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("y", key)) {
      LuaHelper::Read(result.mY, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("z", key)) {
      LuaHelper::Read(result.mZ, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructConstructor.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructConstructor::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructConstructor::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("x", key)) {
    auto res = LuaHelper::Read(instance->mX, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructConstructor.x from lua failed!");
  } else if (0 == strcmp("y", key)) {
    auto res = LuaHelper::Read(instance->mY, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructConstructor.y from lua failed!");
  } else if (0 == strcmp("z", key)) {
    auto res = LuaHelper::Read(instance->mZ, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructConstructor.z from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructConstructor.{}", key);
  }
  return 0;
}

void TestStructConstructor::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructConstructor::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructConstructor::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Construct1");
  lua_pushcfunction(luaState, TestStructConstructor::Construct1CallerFromLua);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "Construct3");
  lua_pushcfunction(luaState, TestStructConstructor::Construct3CallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructConstructor");
}

int TestStructConstructor::Construct1CallerFromLua(lua_State *luaState) {
  float arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = TestStructConstructor::Construct1(arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestStructConstructor::Construct3CallerFromLua(lua_State *luaState) {
  float arg0;
  LuaHelper::Read(arg0, luaState, -3);
  float arg1;
  LuaHelper::Read(arg1, luaState, -2);
  float arg2;
  LuaHelper::Read(arg2, luaState, -1);
  auto result = TestStructConstructor::Construct3(arg0, arg1, arg2);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestStructConstructor::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("x", key)) {
    auto instance = TestStructConstructor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructConstructor.x with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mX, luaState);
  } else if (0 == strcmp("y", key)) {
    auto instance = TestStructConstructor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructConstructor.y with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mY, luaState);
  } else if (0 == strcmp("z", key)) {
    auto instance = TestStructConstructor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructConstructor.z with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mZ, luaState);
  } else if (0 == strcmp("Construct1", key)) {
    lua_pushcfunction(luaState, TestStructConstructor::Construct1CallerFromLua);
  } else if (0 == strcmp("Construct3", key)) {
    lua_pushcfunction(luaState, TestStructConstructor::Construct3CallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructConstructor.{}", key);
    return 0;
  }
  return 1;
}
}
