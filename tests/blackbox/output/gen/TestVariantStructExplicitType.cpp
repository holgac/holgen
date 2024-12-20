// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructExplicitType.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
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

TestVariantStructExplicitType::TestVariantStructExplicitType(TestVariantStructExplicitType &&rhs) noexcept {
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

TestVariantStructExplicitType::~TestVariantStructExplicitType() {
  ResetType();
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

TestVariantStructType TestVariantStructExplicitType::GetType() const {
  return mType;
}

bool TestVariantStructExplicitType::operator==(const TestVariantStructExplicitType &rhs) const {
  if (
      mType != rhs.mType
  ) {
    return false;
  }
  if (mType == TestVariantStructType::Cat) {
    if (!(*GetBeing1AsTestVariantStructCat() == *rhs.GetBeing1AsTestVariantStructCat())) {
      return false;
    }
    if (!(*GetBeing2AsTestVariantStructCat() == *rhs.GetBeing2AsTestVariantStructCat())) {
      return false;
    }
  } else if (mType == TestVariantStructType::Human) {
    if (!(*GetBeing1AsTestVariantStructHuman() == *rhs.GetBeing1AsTestVariantStructHuman())) {
      return false;
    }
    if (!(*GetBeing2AsTestVariantStructHuman() == *rhs.GetBeing2AsTestVariantStructHuman())) {
      return false;
    }
  }
  return true;
}

bool TestVariantStructExplicitType::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("type", name)) {
        TestVariantStructType temp;
        auto res = JsonHelper::Parse(temp, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.type field");
        SetType(temp);
      }
    }
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("type", name)) {
      } else if (0 == strcmp("being1", name)) {
        bool res;
        if (mType == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), data.value, converter, luaState);
        } else if (mType == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), data.value, converter, luaState);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being1 variant field, its type {} is unexpected", mType);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being1 variant field of type {}", mType);
      } else if (0 == strcmp("being2", name)) {
        bool res;
        if (mType == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), data.value, converter, luaState);
        } else if (mType == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), data.value, converter, luaState);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being2 variant field, its type {} is unexpected", mType);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being2 variant field of type {}", mType);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructExplicitType: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructExplicitType!");
      TestVariantStructType temp;
      auto res = JsonHelper::Parse(temp, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.type field");
      SetType(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructExplicitType!");
      bool res;
      if (mType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), (*it), converter, luaState);
      } else if (mType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), (*it), converter, luaState);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being1 variant field, its type {} is unexpected", mType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being1 variant field of type {}", mType);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructExplicitType!");
      bool res;
      if (mType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), (*it), converter, luaState);
      } else if (mType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), (*it), converter, luaState);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructExplicitType.being2 variant field, its type {} is unexpected", mType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructExplicitType.being2 variant field of type {}", mType);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestVariantStructExplicitType!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestVariantStructExplicitType.");
    return false;
  }
  return true;
}

rapidjson::Value TestVariantStructExplicitType::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("type", JsonHelper::Dump(mType, doc, luaState), doc.GetAllocator());
  switch (mType.GetValue()) {
  case TestVariantStructType::Cat:
    val.AddMember("being1", GetBeing1AsTestVariantStructCat()->DumpJson(doc, luaState), doc.GetAllocator());
    val.AddMember("being2", GetBeing2AsTestVariantStructCat()->DumpJson(doc, luaState), doc.GetAllocator());
    break;
  case TestVariantStructType::Human:
    val.AddMember("being1", GetBeing1AsTestVariantStructHuman()->DumpJson(doc, luaState), doc.GetAllocator());
    val.AddMember("being2", GetBeing2AsTestVariantStructHuman()->DumpJson(doc, luaState), doc.GetAllocator());
    break;
  }
  return val;
}

void TestVariantStructExplicitType::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructExplicitType");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructExplicitType::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "type");
  LuaHelper::Push<true>(mType, luaState);
  lua_settable(luaState, -3);
}

void TestVariantStructExplicitType::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructExplicitType *TestVariantStructExplicitType::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestVariantStructExplicitType", className);
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
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestVariantStructExplicitType::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("type", key)) {
    TestVariantStructType temp;
    auto res = LuaHelper::Read(temp, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructExplicitType.type from lua failed!");
    instance->SetType(temp);
  } else if (0 == strcmp("being1", key)) {
    auto res = LuaHelper::Read(instance->mBeing1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructExplicitType.being1 from lua failed!");
  } else if (0 == strcmp("being2", key)) {
    auto res = LuaHelper::Read(instance->mBeing2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructExplicitType.being2 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
  }
  return 0;
}

int TestVariantStructExplicitType::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestVariantStructExplicitType.operator== method with an invalid lua proxy object!");
  TestVariantStructExplicitType arg0Mirror;
  TestVariantStructExplicitType *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestVariantStructExplicitType::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestVariantStructExplicitType::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructExplicitType::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructExplicitType::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestVariantStructExplicitType::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructExplicitType");
}

int TestVariantStructExplicitType::ResetTypeCallerFromLua(lua_State *luaState) {
  auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestVariantStructExplicitType.ResetType method with an invalid lua proxy object!");
  instance->ResetType();
  return 0;
}

int TestVariantStructExplicitType::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("type", key)) {
    auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructExplicitType.type with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mType, luaState);
  } else if (0 == strcmp("being1", key)) {
    auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructExplicitType.being1 with an invalid lua proxy object!");
    switch (instance->mType.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push<false>(instance->GetBeing1AsTestVariantStructCat(), luaState);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push<false>(instance->GetBeing1AsTestVariantStructHuman(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else if (0 == strcmp("being2", key)) {
    auto instance = TestVariantStructExplicitType::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructExplicitType.being2 with an invalid lua proxy object!");
    switch (instance->mType.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push<false>(instance->GetBeing2AsTestVariantStructCat(), luaState);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push<false>(instance->GetBeing2AsTestVariantStructHuman(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else if (0 == strcmp("ResetType", key)) {
    lua_pushcfunction(luaState, TestVariantStructExplicitType::ResetTypeCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructExplicitType.{}", key);
    return 0;
  }
  return 1;
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

TestVariantStructExplicitType &TestVariantStructExplicitType::operator=(TestVariantStructExplicitType &&rhs) noexcept {
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
}
