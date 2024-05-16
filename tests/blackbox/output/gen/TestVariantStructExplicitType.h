// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestVariantStructHuman.h"
#include "TestVariantStructCat.h"
#include "TestVariantStructType.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestVariantStructExplicitType {
public:
  ~TestVariantStructExplicitType();
  bool operator==(const TestVariantStructExplicitType& rhs) const;
  const TestVariantStructType& GetType() const;
  TestVariantStructType& GetType();
  void SetType(const TestVariantStructType& val);
  const TestVariantStructHuman* GetBeing1AsTestVariantStructHuman() const;
  TestVariantStructHuman* GetBeing1AsTestVariantStructHuman();
  const TestVariantStructCat* GetBeing1AsTestVariantStructCat() const;
  TestVariantStructCat* GetBeing1AsTestVariantStructCat();
  const TestVariantStructHuman* GetBeing2AsTestVariantStructHuman() const;
  TestVariantStructHuman* GetBeing2AsTestVariantStructHuman();
  const TestVariantStructCat* GetBeing2AsTestVariantStructCat() const;
  TestVariantStructCat* GetBeing2AsTestVariantStructCat();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static TestVariantStructExplicitType* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  TestVariantStructType mType;
  std::array<uint8_t, std::max({sizeof(TestVariantStructHuman), sizeof(TestVariantStructCat)})> mBeing1;
  std::array<uint8_t, std::max({sizeof(TestVariantStructHuman), sizeof(TestVariantStructCat)})> mBeing2;
};
}
