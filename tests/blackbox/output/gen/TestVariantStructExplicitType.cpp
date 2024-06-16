// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructExplicitType.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestVariantStructExplicitType::TestVariantStructExplicitType(TestVariantStructExplicitType &&rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetType();
}

TestVariantStructExplicitType::TestVariantStructExplicitType(const TestVariantStructExplicitType &rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
}

TestVariantStructExplicitType::~TestVariantStructExplicitType() {
  ResetType();
}

bool TestVariantStructExplicitType::operator==(const TestVariantStructExplicitType &rhs) const {
  return
      mType == rhs.mType;
}

const TestVariantStructType &TestVariantStructExplicitType::GetType() const {
  return mType;
}

TestVariantStructType &TestVariantStructExplicitType::GetType() {
  return mType;
}

void TestVariantStructExplicitType::SetType(const TestVariantStructType &val) {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Invalid, "type field was already initialized (as {}), trying to initialize as {}!,", mType, val);
  mType = val;
  if (val == TestVariantStructType::Cat) {
    new (mBeing1.data()) TestVariantStructCat();
    new (mBeing2.data()) TestVariantStructCat();
  } else if (val == TestVariantStructType::Human) {
    new (mBeing1.data()) TestVariantStructHuman();
    new (mBeing2.data()) TestVariantStructHuman();
  }
}

const TestVariantStructCat *TestVariantStructExplicitType::GetBeing1AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Cat, "Attempting to get TestVariantStructExplicitType.being1 as TestVariantStructCat while its actual type is {}!", mType);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing1.data());
}

TestVariantStructCat *TestVariantStructExplicitType::GetBeing1AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Cat, "Attempting to get TestVariantStructExplicitType.being1 as TestVariantStructCat while its actual type is {}!", mType);
  return reinterpret_cast<TestVariantStructCat *>(mBeing1.data());
}

const TestVariantStructHuman *TestVariantStructExplicitType::GetBeing1AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Human, "Attempting to get TestVariantStructExplicitType.being1 as TestVariantStructHuman while its actual type is {}!", mType);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing1.data());
}

TestVariantStructHuman *TestVariantStructExplicitType::GetBeing1AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Human, "Attempting to get TestVariantStructExplicitType.being1 as TestVariantStructHuman while its actual type is {}!", mType);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing1.data());
}

const TestVariantStructCat *TestVariantStructExplicitType::GetBeing2AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Cat, "Attempting to get TestVariantStructExplicitType.being2 as TestVariantStructCat while its actual type is {}!", mType);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing2.data());
}

TestVariantStructCat *TestVariantStructExplicitType::GetBeing2AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Cat, "Attempting to get TestVariantStructExplicitType.being2 as TestVariantStructCat while its actual type is {}!", mType);
  return reinterpret_cast<TestVariantStructCat *>(mBeing2.data());
}

const TestVariantStructHuman *TestVariantStructExplicitType::GetBeing2AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Human, "Attempting to get TestVariantStructExplicitType.being2 as TestVariantStructHuman while its actual type is {}!", mType);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing2.data());
}

TestVariantStructHuman *TestVariantStructExplicitType::GetBeing2AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mType != TestVariantStructType::Human, "Attempting to get TestVariantStructExplicitType.being2 as TestVariantStructHuman while its actual type is {}!", mType);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing2.data());
}

void TestVariantStructExplicitType::ResetType() {
  if (mType == TestVariantStructType::Invalid) {
    return;
  }
  if (mType == TestVariantStructType::Cat) {
    GetBeing1AsTestVariantStructCat()->~TestVariantStructCat();
    GetBeing2AsTestVariantStructCat()->~TestVariantStructCat();
  } else if (mType == TestVariantStructType::Human) {
    GetBeing1AsTestVariantStructHuman()->~TestVariantStructHuman();
    GetBeing2AsTestVariantStructHuman()->~TestVariantStructHuman();
  }
  mType = TestVariantStructType(TestVariantStructType::Invalid);
}

TestVariantStructExplicitType &TestVariantStructExplicitType::operator=(TestVariantStructExplicitType &&rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetType();
  return *this;
}

TestVariantStructExplicitType &TestVariantStructExplicitType::operator=(const TestVariantStructExplicitType &rhs) {
  ResetType();
  SetType(rhs.mType);
  if (mType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
  return *this;
}

bool TestVariantStructExplicitType::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("type", name)) {
      TestVariantStructType temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.type field");
      SetType(temp);
    }
  }
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestVariantStructExplicitType");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("type", name)) {
    } else if (0 == strcmp("being1", name)) {
      bool res;
      if (mType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), data.value, converter);
      } else if (mType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), data.value, converter);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being1 variant field, its type {} is unexpected", mType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being1 variant field of type {}", mType);
    } else if (0 == strcmp("being2", name)) {
      bool res;
      if (mType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), data.value, converter);
      } else if (mType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), data.value, converter);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being2 variant field, its type {} is unexpected", mType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being2 variant field of type {}", mType);
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructExplicitType: {}", name);
    }
  }
  return true;
}

void TestVariantStructExplicitType::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructExplicitTypeMeta");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructExplicitType::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructExplicitType *TestVariantStructExplicitType::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructExplicitType *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestVariantStructExplicitType TestVariantStructExplicitType::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestVariantStructExplicitType{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("type", key)) {
      LuaHelper::Read(result.mType, luaState, -1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int TestVariantStructExplicitType::IndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    LuaHelper::Push(instance->mType, luaState);
  } else if (0 == strcmp("being1", key)) {
    switch (instance->mType.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push(instance->GetBeing1AsTestVariantStructCat(), luaState);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push(instance->GetBeing1AsTestVariantStructHuman(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else if (0 == strcmp("being2", key)) {
    switch (instance->mType.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push(instance->GetBeing2AsTestVariantStructCat(), luaState);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push(instance->GetBeing2AsTestVariantStructHuman(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
    return 0;
  }
  return 1;
}

int TestVariantStructExplicitType::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    LuaHelper::Read(instance->mType, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
  }
  return 0;
}

void TestVariantStructExplicitType::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructExplicitType::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructExplicitType::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructExplicitTypeMeta");
}
}
