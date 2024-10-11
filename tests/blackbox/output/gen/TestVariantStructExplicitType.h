// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestVariantStructCat.h"
#include "TestVariantStructHuman.h"
#include "TestVariantStructType.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testvariant.hsc
class TestVariantStructExplicitType {
public:
  TestVariantStructExplicitType() = default;
  TestVariantStructExplicitType(const TestVariantStructExplicitType &rhs);
  TestVariantStructExplicitType(TestVariantStructExplicitType &&rhs);
  ~TestVariantStructExplicitType();
  const TestVariantStructCat *GetBeing1AsTestVariantStructCat() const;
  TestVariantStructCat *GetBeing1AsTestVariantStructCat();
  const TestVariantStructHuman *GetBeing1AsTestVariantStructHuman() const;
  TestVariantStructHuman *GetBeing1AsTestVariantStructHuman();
  const TestVariantStructCat *GetBeing2AsTestVariantStructCat() const;
  TestVariantStructCat *GetBeing2AsTestVariantStructCat();
  const TestVariantStructHuman *GetBeing2AsTestVariantStructHuman() const;
  TestVariantStructHuman *GetBeing2AsTestVariantStructHuman();
  void SetType(const TestVariantStructType &val);
  void ResetType();
  TestVariantStructType GetType() const;
  bool operator==(const TestVariantStructExplicitType &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestVariantStructExplicitType *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestVariantStructExplicitType ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestVariantStructExplicitType &operator=(const TestVariantStructExplicitType &rhs);
  TestVariantStructExplicitType &operator=(TestVariantStructExplicitType &&rhs);
  inline static const char *CLASS_NAME = "TestVariantStructExplicitType";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  TestVariantStructType mType;
  std::array<uint8_t, std::max({sizeof(TestVariantStructCat), sizeof(TestVariantStructHuman)})> mBeing1;
  std::array<uint8_t, std::max({sizeof(TestVariantStructCat), sizeof(TestVariantStructHuman)})> mBeing2;
};
}
