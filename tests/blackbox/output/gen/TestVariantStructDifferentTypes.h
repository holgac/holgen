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
class TestVariantStructDifferentTypes {
public:
  TestVariantStructDifferentTypes() = default;
  TestVariantStructDifferentTypes(const TestVariantStructDifferentTypes &rhs);
  TestVariantStructDifferentTypes(TestVariantStructDifferentTypes &&rhs);
  ~TestVariantStructDifferentTypes();
  const TestVariantStructCat *GetBeing1AsTestVariantStructCat() const;
  TestVariantStructCat *GetBeing1AsTestVariantStructCat();
  const TestVariantStructHuman *GetBeing1AsTestVariantStructHuman() const;
  TestVariantStructHuman *GetBeing1AsTestVariantStructHuman();
  const TestVariantStructCat *GetBeing2AsTestVariantStructCat() const;
  TestVariantStructCat *GetBeing2AsTestVariantStructCat();
  const TestVariantStructHuman *GetBeing2AsTestVariantStructHuman() const;
  TestVariantStructHuman *GetBeing2AsTestVariantStructHuman();
  void SetBeing1Type(const TestVariantStructType &val);
  void ResetBeing1Type();
  TestVariantStructType GetBeing1Type() const;
  void SetBeing2Type(const TestVariantStructType &val);
  void ResetBeing2Type();
  TestVariantStructType GetBeing2Type() const;
  bool operator==(const TestVariantStructDifferentTypes &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestVariantStructDifferentTypes *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestVariantStructDifferentTypes ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestVariantStructDifferentTypes &operator=(const TestVariantStructDifferentTypes &rhs);
  TestVariantStructDifferentTypes &operator=(TestVariantStructDifferentTypes &&rhs);
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  TestVariantStructType mBeing1Type;
  std::array<uint8_t, std::max({sizeof(TestVariantStructCat), sizeof(TestVariantStructHuman)})> mBeing1;
  TestVariantStructType mBeing2Type;
  std::array<uint8_t, std::max({sizeof(TestVariantStructCat), sizeof(TestVariantStructHuman)})> mBeing2;
};
}
