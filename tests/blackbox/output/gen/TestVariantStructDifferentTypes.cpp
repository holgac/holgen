// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructDifferentTypes.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestVariantStructDifferentTypes::~TestVariantStructDifferentTypes() {
  if (mBeing1Type == TestVariantStructType::Human) {
    GetBeing1AsTestVariantStructHuman()->~TestVariantStructHuman();
  } else if (mBeing1Type == TestVariantStructType::Cat) {
    GetBeing1AsTestVariantStructCat()->~TestVariantStructCat();
  }
  if (mBeing2Type == TestVariantStructType::Human) {
    GetBeing2AsTestVariantStructHuman()->~TestVariantStructHuman();
  } else if (mBeing2Type == TestVariantStructType::Cat) {
    GetBeing2AsTestVariantStructCat()->~TestVariantStructCat();
  }
}
bool TestVariantStructDifferentTypes::operator==(const TestVariantStructDifferentTypes& rhs) const {
  return true;
}

const TestVariantStructHuman* TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructHuman while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<const TestVariantStructHuman*>(mBeing1.data());
}

TestVariantStructHuman* TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructHuman while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<TestVariantStructHuman*>(mBeing1.data());
}

const TestVariantStructCat* TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructCat while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<const TestVariantStructCat*>(mBeing1.data());
}

TestVariantStructCat* TestVariantStructDifferentTypes::GetBeing1AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being1 as TestVariantStructCat while its actual type is {}!", mBeing1Type);
  return reinterpret_cast<TestVariantStructCat*>(mBeing1.data());
}

const TestVariantStructHuman* TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructHuman() const {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructHuman while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<const TestVariantStructHuman*>(mBeing2.data());
}

TestVariantStructHuman* TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructHuman() {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Human, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructHuman while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<TestVariantStructHuman*>(mBeing2.data());
}

const TestVariantStructCat* TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructCat() const {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructCat while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<const TestVariantStructCat*>(mBeing2.data());
}

TestVariantStructCat* TestVariantStructDifferentTypes::GetBeing2AsTestVariantStructCat() {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Cat, "Attempting to get TestVariantStructDifferentTypes.being2 as TestVariantStructCat while its actual type is {}!", mBeing2Type);
  return reinterpret_cast<TestVariantStructCat*>(mBeing2.data());
}

void TestVariantStructDifferentTypes::SetBeing1Type(const TestVariantStructType& val) {
  HOLGEN_FAIL_IF(mBeing1Type != TestVariantStructType::Invalid, "being1Type field was already initialized (as {}), trying to initialize as {}!,", mBeing1Type, val);
  mBeing1Type = val;
  if (val == TestVariantStructType::Human) {
    new (mBeing1.data()) TestVariantStructHuman();
  } else if (val == TestVariantStructType::Cat) {
    new (mBeing1.data()) TestVariantStructCat();
  }
}

TestVariantStructType TestVariantStructDifferentTypes::GetBeing1Type() const {
  return mBeing1Type;
}

void TestVariantStructDifferentTypes::SetBeing2Type(const TestVariantStructType& val) {
  HOLGEN_FAIL_IF(mBeing2Type != TestVariantStructType::Invalid, "being2Type field was already initialized (as {}), trying to initialize as {}!,", mBeing2Type, val);
  mBeing2Type = val;
  if (val == TestVariantStructType::Human) {
    new (mBeing2.data()) TestVariantStructHuman();
  } else if (val == TestVariantStructType::Cat) {
    new (mBeing2.data()) TestVariantStructCat();
  }
}

TestVariantStructType TestVariantStructDifferentTypes::GetBeing2Type() const {
  return mBeing2Type;
}

bool TestVariantStructDifferentTypes::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  return true;
}

void TestVariantStructDifferentTypes::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructDifferentTypesMeta");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructDifferentTypes::PushGlobalToLua(lua_State* luaState, const char* name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructDifferentTypes* TestVariantStructDifferentTypes::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructDifferentTypes*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestVariantStructDifferentTypes::IndexMetaMethod(lua_State* luaState) {
  return 0;
}

int TestVariantStructDifferentTypes::NewIndexMetaMethod(lua_State* luaState) {
  return 0;
}

void TestVariantStructDifferentTypes::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructDifferentTypes::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructDifferentTypes::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructDifferentTypesMeta");
}
}
