// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructDifferentTypes.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestVariantStructDifferentTypes::TestVariantStructDifferentTypes(const TestVariantStructDifferentTypes &rhs) {
  ResetBeing1Type();
  SetBeing1Type(rhs.mBeing1Type);
  if (mBeing1Type == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
  } else if (mBeing1Type == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
  }
  ResetBeing2Type();
  SetBeing2Type(rhs.mBeing2Type);
  if (mBeing2Type == TestVariantStructType::Cat) {
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mBeing2Type == TestVariantStructType::Human) {
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
  mBeing1Type = rhs.mBeing1Type;
  mBeing2Type = rhs.mBeing2Type;
}

TestVariantStructDifferentTypes::TestVariantStructDifferentTypes(TestVariantStructDifferentTypes &&rhs) noexcept {
  ResetBeing1Type();
  SetBeing1Type(rhs.mBeing1Type);
  if (mBeing1Type == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
  } else if (mBeing1Type == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
  }
  rhs.ResetBeing1Type();
  ResetBeing2Type();
  SetBeing2Type(rhs.mBeing2Type);
  if (mBeing2Type == TestVariantStructType::Cat) {
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mBeing2Type == TestVariantStructType::Human) {
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetBeing2Type();
  mBeing1Type = std::move(rhs.mBeing1Type);
  mBeing2Type = std::move(rhs.mBeing2Type);
}

TestVariantStructDifferentTypes::~TestVariantStructDifferentTypes() {
  ResetBeing1Type();
  ResetBeing2Type();
}

const TestVariantStructCat *TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructCat while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing1.data());
}

TestVariantStructCat *TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructCat while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<TestVariantStructCat *>(mBeing1.data());
}

const TestVariantStructHuman *TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructHuman while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing1.data());
}

TestVariantStructHuman *TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructHuman while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing1.data());
}

const TestVariantStructCat *TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructCat while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing2.data());
}

TestVariantStructCat *TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructCat while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<TestVariantStructCat *>(mBeing2.data());
}

const TestVariantStructHuman *TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructHuman while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing2.data());
}

TestVariantStructHuman *TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructHuman while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing2.data());
}

void TestVariantStructDifferentTypes::SetBeing1Type(const TestVariantStructType &val) {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Invalid, "being1Type field was already initialized (as {}), trying to initialize as {}!,", mBeing1Type, val);
  mBeing1Type = val;
  if (val == TestVariantStructType::Cat) {
    new (mBeing1.data()) TestVariantStructCat();
  } else if (val == TestVariantStructType::Human) {
    new (mBeing1.data()) TestVariantStructHuman();
  }
}

void TestVariantStructDifferentTypes::ResetBeing1Type() {
  if (mBeing1Type == TestVariantStructType::Invalid) {
    return;
  }
  if (mBeing1Type == TestVariantStructType::Cat) {
    GetBeing1AsTestVariantStructCat()->~TestVariantStructCat();
  } else if (mBeing1Type == TestVariantStructType::Human) {
    GetBeing1AsTestVariantStructHuman()->~TestVariantStructHuman();
  }
  mBeing1Type = TestVariantStructType(TestVariantStructType::Invalid);
}

TestVariantStructType TestVariantStructDifferentTypes::GetBeing1Type() const {
  return mBeing1Type;
}

void TestVariantStructDifferentTypes::SetBeing2Type(const TestVariantStructType &val) {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Invalid, "being2Type field was already initialized (as {}), trying to initialize as {}!,", mBeing2Type, val);
  mBeing2Type = val;
  if (val == TestVariantStructType::Cat) {
    new (mBeing2.data()) TestVariantStructCat();
  } else if (val == TestVariantStructType::Human) {
    new (mBeing2.data()) TestVariantStructHuman();
  }
}

void TestVariantStructDifferentTypes::ResetBeing2Type() {
  if (mBeing2Type == TestVariantStructType::Invalid) {
    return;
  }
  if (mBeing2Type == TestVariantStructType::Cat) {
    GetBeing2AsTestVariantStructCat()->~TestVariantStructCat();
  } else if (mBeing2Type == TestVariantStructType::Human) {
    GetBeing2AsTestVariantStructHuman()->~TestVariantStructHuman();
  }
  mBeing2Type = TestVariantStructType(TestVariantStructType::Invalid);
}

TestVariantStructType TestVariantStructDifferentTypes::GetBeing2Type() const {
  return mBeing2Type;
}

bool TestVariantStructDifferentTypes::operator==(const TestVariantStructDifferentTypes &rhs) const {
  if (
      mBeing1Type != rhs.mBeing1Type ||
      mBeing2Type != rhs.mBeing2Type
  ) {
    return false;
  }
  if (mBeing1Type == TestVariantStructType::Cat) {
    if (!(*GetBeing1AsTestVariantStructCat() == *rhs.GetBeing1AsTestVariantStructCat())) {
      return false;
    }
  } else if (mBeing1Type == TestVariantStructType::Human) {
    if (!(*GetBeing1AsTestVariantStructHuman() == *rhs.GetBeing1AsTestVariantStructHuman())) {
      return false;
    }
  }
  return true;
  if (mBeing2Type == TestVariantStructType::Cat) {
    if (!(*GetBeing2AsTestVariantStructCat() == *rhs.GetBeing2AsTestVariantStructCat())) {
      return false;
    }
  } else if (mBeing2Type == TestVariantStructType::Human) {
    if (!(*GetBeing2AsTestVariantStructHuman() == *rhs.GetBeing2AsTestVariantStructHuman())) {
      return false;
    }
  }
  return true;
}

bool TestVariantStructDifferentTypes::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("being1Type", name)) {
        TestVariantStructType temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being1Type field");
        SetBeing1Type(temp);
      } else if (0 == strcmp("being2Type", name)) {
        TestVariantStructType temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being2Type field");
        SetBeing2Type(temp);
      }
    }
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("being1Type", name)) {
      } else if (0 == strcmp("being1", name)) {
        bool res;
        if (mBeing1Type == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), data.value, converter);
        } else if (mBeing1Type == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), data.value, converter);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructDifferentTypes.being1 variant field, its type {} is unexpected", mBeing1Type);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being1 variant field of type {}", mBeing1Type);
      } else if (0 == strcmp("being2Type", name)) {
      } else if (0 == strcmp("being2", name)) {
        bool res;
        if (mBeing2Type == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), data.value, converter);
        } else if (mBeing2Type == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), data.value, converter);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructDifferentTypes.being2 variant field, its type {} is unexpected", mBeing2Type);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being2 variant field of type {}", mBeing2Type);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructDifferentTypes: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructDifferentTypes!");
      TestVariantStructType temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being1Type field");
      SetBeing1Type(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructDifferentTypes!");
      bool res;
      if (mBeing1Type == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), (*it), converter);
      } else if (mBeing1Type == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), (*it), converter);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructDifferentTypes.being1 variant field, its type {} is unexpected", mBeing1Type);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being1 variant field of type {}", mBeing1Type);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructDifferentTypes!");
      TestVariantStructType temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being2Type field");
      SetBeing2Type(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructDifferentTypes!");
      bool res;
      if (mBeing2Type == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), (*it), converter);
      } else if (mBeing2Type == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), (*it), converter);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructDifferentTypes.being2 variant field, its type {} is unexpected", mBeing2Type);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructDifferentTypes.being2 variant field of type {}", mBeing2Type);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestVariantStructDifferentTypes!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestVariantStructDifferentTypes.");
    return false;
  }
  return true;
}

void TestVariantStructDifferentTypes::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructDifferentTypesMeta");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructDifferentTypes::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
}

void TestVariantStructDifferentTypes::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructDifferentTypes *TestVariantStructDifferentTypes::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestVariantStructDifferentTypes", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructDifferentTypes *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestVariantStructDifferentTypes TestVariantStructDifferentTypes::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  return TestVariantStructDifferentTypes{};
}

int TestVariantStructDifferentTypes::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestVariantStructDifferentTypes::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructDifferentTypes::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructDifferentTypes::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructDifferentTypesMeta");
}

int TestVariantStructDifferentTypes::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("being1", key)) {
    auto instance = TestVariantStructDifferentTypes::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructDifferentTypes.being1 with an invalid lua proxy object!");
    switch (instance->mBeing1Type.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push(instance->GetBeing1AsTestVariantStructCat(), luaState, false);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push(instance->GetBeing1AsTestVariantStructHuman(), luaState, false);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else if (0 == strcmp("being2", key)) {
    auto instance = TestVariantStructDifferentTypes::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructDifferentTypes.being2 with an invalid lua proxy object!");
    switch (instance->mBeing2Type.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push(instance->GetBeing2AsTestVariantStructCat(), luaState, false);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push(instance->GetBeing2AsTestVariantStructHuman(), luaState, false);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructDifferentTypes.{}", key);
    return 0;
  }
  return 1;
}

TestVariantStructDifferentTypes &TestVariantStructDifferentTypes::operator=(const TestVariantStructDifferentTypes &rhs) {
  ResetBeing1Type();
  SetBeing1Type(rhs.mBeing1Type);
  if (mBeing1Type == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
  } else if (mBeing1Type == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
  }
  ResetBeing2Type();
  SetBeing2Type(rhs.mBeing2Type);
  if (mBeing2Type == TestVariantStructType::Cat) {
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mBeing2Type == TestVariantStructType::Human) {
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
  mBeing1Type = rhs.mBeing1Type;
  mBeing2Type = rhs.mBeing2Type;
  return *this;
}

TestVariantStructDifferentTypes &TestVariantStructDifferentTypes::operator=(TestVariantStructDifferentTypes &&rhs) noexcept {
  ResetBeing1Type();
  SetBeing1Type(rhs.mBeing1Type);
  if (mBeing1Type == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
  } else if (mBeing1Type == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
  }
  rhs.ResetBeing1Type();
  ResetBeing2Type();
  SetBeing2Type(rhs.mBeing2Type);
  if (mBeing2Type == TestVariantStructType::Cat) {
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mBeing2Type == TestVariantStructType::Human) {
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetBeing2Type();
  mBeing1Type = std::move(rhs.mBeing1Type);
  mBeing2Type = std::move(rhs.mBeing2Type);
  return *this;
}
}
