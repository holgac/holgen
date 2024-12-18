// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructSharedType.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestVariantStructSharedType::TestVariantStructSharedType(const TestVariantStructSharedType &rhs) {
  ResetBeingType();
  SetBeingType(rhs.mBeingType);
  if (mBeingType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mBeingType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
  mBeingType = rhs.mBeingType;
}

TestVariantStructSharedType::TestVariantStructSharedType(TestVariantStructSharedType &&rhs) noexcept {
  ResetBeingType();
  SetBeingType(rhs.mBeingType);
  if (mBeingType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mBeingType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetBeingType();
  mBeingType = std::move(rhs.mBeingType);
}

TestVariantStructSharedType::~TestVariantStructSharedType() {
  ResetBeingType();
}

const TestVariantStructCat *TestVariantStructSharedType::GetBeing1AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Cat, "Attempting to get TestVariantStructSharedType.being1 as TestVariantStructCat while its actual type is {}!", mBeingType);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing1.data());
}

TestVariantStructCat *TestVariantStructSharedType::GetBeing1AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Cat, "Attempting to get TestVariantStructSharedType.being1 as TestVariantStructCat while its actual type is {}!", mBeingType);
  return reinterpret_cast<TestVariantStructCat *>(mBeing1.data());
}

const TestVariantStructHuman *TestVariantStructSharedType::GetBeing1AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Human, "Attempting to get TestVariantStructSharedType.being1 as TestVariantStructHuman while its actual type is {}!", mBeingType);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing1.data());
}

TestVariantStructHuman *TestVariantStructSharedType::GetBeing1AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Human, "Attempting to get TestVariantStructSharedType.being1 as TestVariantStructHuman while its actual type is {}!", mBeingType);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing1.data());
}

const TestVariantStructCat *TestVariantStructSharedType::GetBeing2AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Cat, "Attempting to get TestVariantStructSharedType.being2 as TestVariantStructCat while its actual type is {}!", mBeingType);
  return reinterpret_cast<const TestVariantStructCat *>(mBeing2.data());
}

TestVariantStructCat *TestVariantStructSharedType::GetBeing2AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Cat, "Attempting to get TestVariantStructSharedType.being2 as TestVariantStructCat while its actual type is {}!", mBeingType);
  return reinterpret_cast<TestVariantStructCat *>(mBeing2.data());
}

const TestVariantStructHuman *TestVariantStructSharedType::GetBeing2AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Human, "Attempting to get TestVariantStructSharedType.being2 as TestVariantStructHuman while its actual type is {}!", mBeingType);
  return reinterpret_cast<const TestVariantStructHuman *>(mBeing2.data());
}

TestVariantStructHuman *TestVariantStructSharedType::GetBeing2AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Human, "Attempting to get TestVariantStructSharedType.being2 as TestVariantStructHuman while its actual type is {}!", mBeingType);
  return reinterpret_cast<TestVariantStructHuman *>(mBeing2.data());
}

void TestVariantStructSharedType::SetBeingType(const TestVariantStructType &val) {
  HOLGEN_FAIL_IF(mBeingType != TestVariantStructType::Invalid, "beingType field was already initialized (as {}), trying to initialize as {}!,", mBeingType, val);
  mBeingType = val;
  if (val == TestVariantStructType::Cat) {
    new (mBeing1.data()) TestVariantStructCat();
    new (mBeing2.data()) TestVariantStructCat();
  } else if (val == TestVariantStructType::Human) {
    new (mBeing1.data()) TestVariantStructHuman();
    new (mBeing2.data()) TestVariantStructHuman();
  }
}

void TestVariantStructSharedType::ResetBeingType() {
  if (mBeingType == TestVariantStructType::Invalid) {
    return;
  }
  if (mBeingType == TestVariantStructType::Cat) {
    GetBeing1AsTestVariantStructCat()->~TestVariantStructCat();
    GetBeing2AsTestVariantStructCat()->~TestVariantStructCat();
  } else if (mBeingType == TestVariantStructType::Human) {
    GetBeing1AsTestVariantStructHuman()->~TestVariantStructHuman();
    GetBeing2AsTestVariantStructHuman()->~TestVariantStructHuman();
  }
  mBeingType = TestVariantStructType(TestVariantStructType::Invalid);
}

TestVariantStructType TestVariantStructSharedType::GetBeingType() const {
  return mBeingType;
}

bool TestVariantStructSharedType::operator==(const TestVariantStructSharedType &rhs) const {
  if (
      mBeingType != rhs.mBeingType
  ) {
    return false;
  }
  if (mBeingType == TestVariantStructType::Cat) {
    if (!(*GetBeing1AsTestVariantStructCat() == *rhs.GetBeing1AsTestVariantStructCat())) {
      return false;
    }
    if (!(*GetBeing2AsTestVariantStructCat() == *rhs.GetBeing2AsTestVariantStructCat())) {
      return false;
    }
  } else if (mBeingType == TestVariantStructType::Human) {
    if (!(*GetBeing1AsTestVariantStructHuman() == *rhs.GetBeing1AsTestVariantStructHuman())) {
      return false;
    }
    if (!(*GetBeing2AsTestVariantStructHuman() == *rhs.GetBeing2AsTestVariantStructHuman())) {
      return false;
    }
  }
  return true;
}

bool TestVariantStructSharedType::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("beingType", name)) {
        TestVariantStructType temp;
        auto res = JsonHelper::Parse(temp, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.beingType field");
        SetBeingType(temp);
      }
    }
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("beingType", name)) {
      } else if (0 == strcmp("being1", name)) {
        bool res;
        if (mBeingType == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), data.value, converter, luaState);
        } else if (mBeingType == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), data.value, converter, luaState);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructSharedType.being1 variant field, its type {} is unexpected", mBeingType);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.being1 variant field of type {}", mBeingType);
      } else if (0 == strcmp("being2", name)) {
        bool res;
        if (mBeingType == TestVariantStructType::Cat) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), data.value, converter, luaState);
        } else if (mBeingType == TestVariantStructType::Human) {
          res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), data.value, converter, luaState);
        } else {
          HOLGEN_WARN("Could not json-parse TestVariantStructSharedType.being2 variant field, its type {} is unexpected", mBeingType);
          return false;
        }
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.being2 variant field of type {}", mBeingType);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructSharedType: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructSharedType!");
      TestVariantStructType temp;
      auto res = JsonHelper::Parse(temp, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.beingType field");
      SetBeingType(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructSharedType!");
      bool res;
      if (mBeingType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructCat(), (*it), converter, luaState);
      } else if (mBeingType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing1AsTestVariantStructHuman(), (*it), converter, luaState);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructSharedType.being1 variant field, its type {} is unexpected", mBeingType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.being1 variant field of type {}", mBeingType);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructSharedType!");
      bool res;
      if (mBeingType == TestVariantStructType::Cat) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructCat(), (*it), converter, luaState);
      } else if (mBeingType == TestVariantStructType::Human) {
        res = JsonHelper::Parse(*GetBeing2AsTestVariantStructHuman(), (*it), converter, luaState);
      } else {
        HOLGEN_WARN("Could not json-parse TestVariantStructSharedType.being2 variant field, its type {} is unexpected", mBeingType);
        return false;
      }
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructSharedType.being2 variant field of type {}", mBeingType);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestVariantStructSharedType!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestVariantStructSharedType.");
    return false;
  }
  return true;
}

rapidjson::Value TestVariantStructSharedType::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("beingType", JsonHelper::Dump(mBeingType, doc, luaState), doc.GetAllocator());
  switch (mBeingType.GetValue()) {
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

void TestVariantStructSharedType::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructSharedType");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructSharedType::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
}

void TestVariantStructSharedType::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructSharedType *TestVariantStructSharedType::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestVariantStructSharedType", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructSharedType *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestVariantStructSharedType TestVariantStructSharedType::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  return TestVariantStructSharedType{};
}

int TestVariantStructSharedType::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructSharedType::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("being1", key)) {
    auto res = LuaHelper::Read(instance->mBeing1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructSharedType.being1 from lua failed!");
  } else if (0 == strcmp("being2", key)) {
    auto res = LuaHelper::Read(instance->mBeing2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructSharedType.being2 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructSharedType.{}", key);
  }
  return 0;
}

int TestVariantStructSharedType::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestVariantStructSharedType::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestVariantStructSharedType.operator== method with an invalid lua proxy object!");
  TestVariantStructSharedType arg0Mirror;
  TestVariantStructSharedType *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestVariantStructSharedType::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestVariantStructSharedType::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestVariantStructSharedType::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructSharedType::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructSharedType::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestVariantStructSharedType::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructSharedType");
}

int TestVariantStructSharedType::ResetBeingTypeCallerFromLua(lua_State *luaState) {
  auto instance = TestVariantStructSharedType::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestVariantStructSharedType.ResetBeingType method with an invalid lua proxy object!");
  instance->ResetBeingType();
  return 0;
}

int TestVariantStructSharedType::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("being1", key)) {
    auto instance = TestVariantStructSharedType::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructSharedType.being1 with an invalid lua proxy object!");
    switch (instance->mBeingType.GetValue()) {
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
    auto instance = TestVariantStructSharedType::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructSharedType.being2 with an invalid lua proxy object!");
    switch (instance->mBeingType.GetValue()) {
    case TestVariantStructType::Cat:
      LuaHelper::Push<false>(instance->GetBeing2AsTestVariantStructCat(), luaState);
      break;
    case TestVariantStructType::Human:
      LuaHelper::Push<false>(instance->GetBeing2AsTestVariantStructHuman(), luaState);
      break;
    default:
      lua_pushnil(luaState);
    }
  } else if (0 == strcmp("ResetBeingType", key)) {
    lua_pushcfunction(luaState, TestVariantStructSharedType::ResetBeingTypeCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructSharedType.{}", key);
    return 0;
  }
  return 1;
}

TestVariantStructSharedType &TestVariantStructSharedType::operator=(const TestVariantStructSharedType &rhs) {
  ResetBeingType();
  SetBeingType(rhs.mBeingType);
  if (mBeingType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = *rhs.GetBeing1AsTestVariantStructCat();
    *GetBeing2AsTestVariantStructCat() = *rhs.GetBeing2AsTestVariantStructCat();
  } else if (mBeingType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = *rhs.GetBeing1AsTestVariantStructHuman();
    *GetBeing2AsTestVariantStructHuman() = *rhs.GetBeing2AsTestVariantStructHuman();
  }
  mBeingType = rhs.mBeingType;
  return *this;
}

TestVariantStructSharedType &TestVariantStructSharedType::operator=(TestVariantStructSharedType &&rhs) noexcept {
  ResetBeingType();
  SetBeingType(rhs.mBeingType);
  if (mBeingType == TestVariantStructType::Cat) {
    *GetBeing1AsTestVariantStructCat() = std::move(*rhs.GetBeing1AsTestVariantStructCat());
    *GetBeing2AsTestVariantStructCat() = std::move(*rhs.GetBeing2AsTestVariantStructCat());
  } else if (mBeingType == TestVariantStructType::Human) {
    *GetBeing1AsTestVariantStructHuman() = std::move(*rhs.GetBeing1AsTestVariantStructHuman());
    *GetBeing2AsTestVariantStructHuman() = std::move(*rhs.GetBeing2AsTestVariantStructHuman());
  }
  rhs.ResetBeingType();
  mBeingType = std::move(rhs.mBeingType);
  return *this;
}
}
