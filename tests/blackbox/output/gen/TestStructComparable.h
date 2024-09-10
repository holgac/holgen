// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestStructComparable {
public:
  TestStructComparable() = default;
  TestStructComparable(const TestStructComparable &rhs) = default;
  TestStructComparable(TestStructComparable &&rhs) = default;
  ~TestStructComparable() = default;
  bool operator==(const TestStructComparable &rhs) const;
  uint32_t GetField1() const;
  uint32_t GetField2() const;
  void SetField1(uint32_t val);
  void SetField2(uint32_t val);
  bool operator<(const TestStructComparable &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructComparable *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructComparable ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructComparable &operator=(const TestStructComparable &rhs) = default;
  TestStructComparable &operator=(TestStructComparable &&rhs) = default;
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mField1;
  uint32_t mField2;
};
}
