// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestEnum.h"
#include "TestEnumDefaultValue.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testenum.hsc
class TestEnumStruct {
public:
  TestEnumStruct() = default;
  TestEnumStruct(const TestEnumStruct &rhs) = default;
  TestEnumStruct(TestEnumStruct &&rhs) noexcept = default;
  ~TestEnumStruct() = default;
  bool operator==(const TestEnumStruct &rhs) const;
  const TestEnum &GetEnumField() const;
  TestEnum &GetEnumField();
  const TestEnumDefaultValue &GetEnumDefaultValueField() const;
  TestEnumDefaultValue &GetEnumDefaultValueField();
  void SetEnumField(const TestEnum &val);
  void SetEnumDefaultValueField(const TestEnumDefaultValue &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestEnumStruct *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestEnumStruct ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestEnumStruct &operator=(const TestEnumStruct &rhs) = default;
  TestEnumStruct &operator=(TestEnumStruct &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestEnumStruct";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  TestEnum mEnumField;
  TestEnumDefaultValue mEnumDefaultValueField;
};
}
