// This file is generated by holgen. Do not modify manually.
#include "TestStructArrayCustomData1.h"

#include <cmath>
#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
uint32_t TestStructArrayCustomData1::GetF1() const {
  return mF1;
}

float TestStructArrayCustomData1::GetF2() const {
  return mF2;
}

uint64_t TestStructArrayCustomData1::GetF3() const {
  return mF3;
}

void TestStructArrayCustomData1::SetF1(uint32_t val) {
  mF1 = val;
}

void TestStructArrayCustomData1::SetF2(float val) {
  mF2 = val;
}

void TestStructArrayCustomData1::SetF3(uint64_t val) {
  mF3 = val;
}

bool TestStructArrayCustomData1::operator==(const TestStructArrayCustomData1 &rhs) const {
  return !(
      mF1 != rhs.mF1 ||
      std::fabs(mF2 - rhs.mF2) >= 0.00001 ||
      mF3 != rhs.mF3
  );
}

bool TestStructArrayCustomData1::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("f1", name)) {
        auto res = JsonHelper::Parse(mF1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f1 field");
      } else if (0 == strcmp("f2", name)) {
        auto res = JsonHelper::Parse(mF2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f2 field");
      } else if (0 == strcmp("f3", name)) {
        auto res = JsonHelper::Parse(mF3, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f3 field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructArrayCustomData1: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData1!");
      auto res = JsonHelper::Parse(mF1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData1!");
      auto res = JsonHelper::Parse(mF2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f2 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructArrayCustomData1!");
      auto res = JsonHelper::Parse(mF3, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructArrayCustomData1.f3 field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructArrayCustomData1!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructArrayCustomData1.");
    return false;
  }
  return true;
}

rapidjson::Value TestStructArrayCustomData1::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("f1", JsonHelper::Dump(mF1, doc), doc.GetAllocator());
  val.AddMember("f2", JsonHelper::Dump(mF2, doc), doc.GetAllocator());
  val.AddMember("f3", JsonHelper::Dump(mF3, doc), doc.GetAllocator());
  return val;
}

void TestStructArrayCustomData1::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructArrayCustomData1");
  lua_setmetatable(luaState, -2);
}

void TestStructArrayCustomData1::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "f1");
  LuaHelper::Push<true>(mF1, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "f2");
  LuaHelper::Push<true>(mF2, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "f3");
  LuaHelper::Push<true>(mF3, luaState);
  lua_settable(luaState, -3);
}

void TestStructArrayCustomData1::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructArrayCustomData1 *TestStructArrayCustomData1::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructArrayCustomData1", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructArrayCustomData1 *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructArrayCustomData1 TestStructArrayCustomData1::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructArrayCustomData1{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("f1", key)) {
      LuaHelper::Read(result.mF1, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("f2", key)) {
      LuaHelper::Read(result.mF2, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("f3", key)) {
      LuaHelper::Read(result.mF3, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData1.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructArrayCustomData1::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructArrayCustomData1::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("f1", key)) {
    auto res = LuaHelper::Read(instance->mF1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData1.f1 from lua failed!");
  } else if (0 == strcmp("f2", key)) {
    auto res = LuaHelper::Read(instance->mF2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData1.f2 from lua failed!");
  } else if (0 == strcmp("f3", key)) {
    auto res = LuaHelper::Read(instance->mF3, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructArrayCustomData1.f3 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData1.{}", key);
  }
  return 0;
}

void TestStructArrayCustomData1::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructArrayCustomData1::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructArrayCustomData1::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructArrayCustomData1");
}

int TestStructArrayCustomData1::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("f1", key)) {
    auto instance = TestStructArrayCustomData1::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData1.f1 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mF1, luaState);
  } else if (0 == strcmp("f2", key)) {
    auto instance = TestStructArrayCustomData1::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData1.f2 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mF2, luaState);
  } else if (0 == strcmp("f3", key)) {
    auto instance = TestStructArrayCustomData1::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructArrayCustomData1.f3 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mF3, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructArrayCustomData1.{}", key);
    return 0;
  }
  return 1;
}
}
