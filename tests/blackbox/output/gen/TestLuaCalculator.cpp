// This file is generated by holgen. Do not modify manually.
#include "TestLuaCalculator.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const TestLuaNumber &TestLuaCalculator::GetLastValue() const {
  return mLastValue;
}

TestLuaNumber &TestLuaCalculator::GetLastValue() {
  return mLastValue;
}

void TestLuaCalculator::SetLastValue(const TestLuaNumber &val) {
  mLastValue = val;
}

int32_t TestLuaCalculator::AddPrimitive(lua_State *luaState, const int32_t num) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_AddPrimitive.empty(), {}, "Calling unset AddPrimitive function");
  lua_getglobal(luaState, mLuaFuncHandle_AddPrimitive.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined AddPrimitive function {}", mLuaFuncHandle_AddPrimitive);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

void TestLuaCalculator::SetAddPrimitiveLuaFunc(std::string val) {
  mLuaFuncHandle_AddPrimitive = val;
}

bool TestLuaCalculator::HasAddPrimitiveLuaFunc() const {
  return !mLuaFuncHandle_AddPrimitive.empty();
}

int32_t TestLuaCalculator::AddRef(lua_State *luaState, TestLuaNumber &num) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_AddRef.empty(), {}, "Calling unset AddRef function");
  lua_getglobal(luaState, mLuaFuncHandle_AddRef.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined AddRef function {}", mLuaFuncHandle_AddRef);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

void TestLuaCalculator::SetAddRefLuaFunc(std::string val) {
  mLuaFuncHandle_AddRef = val;
}

bool TestLuaCalculator::HasAddRefLuaFunc() const {
  return !mLuaFuncHandle_AddRef.empty();
}

int32_t TestLuaCalculator::AddNullable(lua_State *luaState, const TestLuaNumber *num) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_AddNullable.empty(), {}, "Calling unset AddNullable function");
  lua_getglobal(luaState, mLuaFuncHandle_AddNullable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined AddNullable function {}", mLuaFuncHandle_AddNullable);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

void TestLuaCalculator::SetAddNullableLuaFunc(std::string val) {
  mLuaFuncHandle_AddNullable = val;
}

bool TestLuaCalculator::HasAddNullableLuaFunc() const {
  return !mLuaFuncHandle_AddNullable.empty();
}

TestLuaNumber *TestLuaCalculator::ReturnNullable(lua_State *luaState, const int32_t num) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_ReturnNullable.empty(), nullptr, "Calling unset ReturnNullable function");
  lua_getglobal(luaState, mLuaFuncHandle_ReturnNullable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined ReturnNullable function {}", mLuaFuncHandle_ReturnNullable);
    lua_pop(luaState, 1);
    return nullptr;
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  auto result = TestLuaNumber::ReadProxyFromLua(luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

void TestLuaCalculator::SetReturnNullableLuaFunc(std::string val) {
  mLuaFuncHandle_ReturnNullable = val;
}

bool TestLuaCalculator::HasReturnNullableLuaFunc() const {
  return !mLuaFuncHandle_ReturnNullable.empty();
}

TestLuaNumber &TestLuaCalculator::ReturnRef(lua_State *luaState, const int32_t num) const {
  HOLGEN_FAIL_IF(mLuaFuncHandle_ReturnRef.empty(), "Calling unset ReturnRef function");
  lua_getglobal(luaState, mLuaFuncHandle_ReturnRef.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_FAIL("Calling undefined ReturnRef function {}", mLuaFuncHandle_ReturnRef);
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  auto result = TestLuaNumber::ReadProxyFromLua(luaState, -1);
  lua_pop(luaState, 1);
  return *result;
}

void TestLuaCalculator::SetReturnRefLuaFunc(std::string val) {
  mLuaFuncHandle_ReturnRef = val;
}

bool TestLuaCalculator::HasReturnRefLuaFunc() const {
  return !mLuaFuncHandle_ReturnRef.empty();
}

TestLuaNumber TestLuaCalculator::ReturnNew(lua_State *luaState, const int32_t num) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_ReturnNew.empty(), {}, "Calling unset ReturnNew function");
  lua_getglobal(luaState, mLuaFuncHandle_ReturnNew.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined ReturnNew function {}", mLuaFuncHandle_ReturnNew);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(num, luaState);
  lua_call(luaState, 2, 1);
  TestLuaNumber resultMirror;
  TestLuaNumber *result;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    result = TestLuaNumber::ReadProxyFromLua(luaState, -1);
  } else {
    resultMirror = TestLuaNumber::ReadMirrorFromLua(luaState, -1);
    result = &resultMirror;
  }
  lua_pop(luaState, 1);
  return *result;
}

void TestLuaCalculator::SetReturnNewLuaFunc(std::string val) {
  mLuaFuncHandle_ReturnNew = val;
}

bool TestLuaCalculator::HasReturnNewLuaFunc() const {
  return !mLuaFuncHandle_ReturnNew.empty();
}

bool TestLuaCalculator::operator==(const TestLuaCalculator &rhs) const {
  return !(
      mLastValue != rhs.mLastValue ||
      mLuaFuncHandle_AddPrimitive != rhs.mLuaFuncHandle_AddPrimitive ||
      mLuaFuncHandle_AddRef != rhs.mLuaFuncHandle_AddRef ||
      mLuaFuncHandle_AddNullable != rhs.mLuaFuncHandle_AddNullable ||
      mLuaFuncHandle_ReturnNullable != rhs.mLuaFuncHandle_ReturnNullable ||
      mLuaFuncHandle_ReturnRef != rhs.mLuaFuncHandle_ReturnRef ||
      mLuaFuncHandle_ReturnNew != rhs.mLuaFuncHandle_ReturnNew
  );
}

bool TestLuaCalculator::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("lastValue", name)) {
        auto res = JsonHelper::Parse(mLastValue, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.lastValue field");
      } else if (0 == strcmp("AddPrimitive", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_AddPrimitive, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddPrimitive");
      } else if (0 == strcmp("AddRef", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_AddRef, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddRef");
      } else if (0 == strcmp("AddNullable", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_AddNullable, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddNullable");
      } else if (0 == strcmp("ReturnNullable", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnNullable, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnNullable");
      } else if (0 == strcmp("ReturnRef", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnRef, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnRef");
      } else if (0 == strcmp("ReturnNew", name)) {
        auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnNew, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnNew");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestLuaCalculator: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaCalculator!");
      auto res = JsonHelper::Parse(mLastValue, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.lastValue field");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_AddPrimitive, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddPrimitive");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_AddRef, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddRef");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_AddNullable, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.AddNullable");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnNullable, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnNullable");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnRef, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnRef");
      ++it;
    }
    {
      auto res = JsonHelper::Parse(mLuaFuncHandle_ReturnNew, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaCalculator.ReturnNew");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestLuaCalculator!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestLuaCalculator.");
    return false;
  }
  return true;
}

rapidjson::Value TestLuaCalculator::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("lastValue", JsonHelper::Dump(mLastValue, doc, luaState), doc.GetAllocator());
  return val;
}

void TestLuaCalculator::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaCalculator");
  lua_setmetatable(luaState, -2);
}

void TestLuaCalculator::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "lastValue");
  mLastValue.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
}

void TestLuaCalculator::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaCalculator *TestLuaCalculator::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestLuaCalculator", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaCalculator *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaCalculator TestLuaCalculator::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestLuaCalculator{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("lastValue", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mLastValue = *TestLuaNumber::ReadProxyFromLua(luaState, -1);
      } else {
        result.mLastValue = TestLuaNumber::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestLuaCalculator.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestLuaCalculator::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("lastValue", key)) {
    auto res = LuaHelper::Read(instance->mLastValue, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaCalculator.lastValue from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaCalculator.{}", key);
  }
  return 0;
}

int TestLuaCalculator::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.operator== method with an invalid lua proxy object!");
  TestLuaCalculator arg0Mirror;
  TestLuaCalculator *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestLuaCalculator::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestLuaCalculator::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestLuaCalculator::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaCalculator::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaCalculator::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestLuaCalculator::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaCalculator");
}

int TestLuaCalculator::AddPrimitiveCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.AddPrimitive method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->AddPrimitive(luaState, arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaCalculator::AddRefCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.AddRef method with an invalid lua proxy object!");
  auto arg0 = TestLuaNumber::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddRef(luaState, *arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaCalculator::AddNullableCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.AddNullable method with an invalid lua proxy object!");
  TestLuaNumber arg0Mirror;
  TestLuaNumber *arg0;
  if (lua_isnil(luaState, -1)) {
    arg0 = nullptr;
  } else if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestLuaNumber::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestLuaNumber::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->AddNullable(luaState, arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaCalculator::ReturnNullableCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.ReturnNullable method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->ReturnNullable(luaState, arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestLuaCalculator::ReturnRefCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.ReturnRef method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto& result = instance->ReturnRef(luaState, arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestLuaCalculator::ReturnNewCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaCalculator.ReturnNew method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->ReturnNew(luaState, arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaCalculator::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("lastValue", key)) {
    auto instance = TestLuaCalculator::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaCalculator.lastValue with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mLastValue, luaState);
  } else if (0 == strcmp("AddPrimitive", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::AddPrimitiveCallerFromLua);
  } else if (0 == strcmp("AddRef", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::AddRefCallerFromLua);
  } else if (0 == strcmp("AddNullable", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::AddNullableCallerFromLua);
  } else if (0 == strcmp("ReturnNullable", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::ReturnNullableCallerFromLua);
  } else if (0 == strcmp("ReturnRef", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::ReturnRefCallerFromLua);
  } else if (0 == strcmp("ReturnNew", key)) {
    lua_pushcfunction(luaState, TestLuaCalculator::ReturnNewCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaCalculator.{}", key);
    return 0;
  }
  return 1;
}
}
