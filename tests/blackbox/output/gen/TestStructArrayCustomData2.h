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
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructArrayCustomData2 {
public:
  TestStructArrayCustomData2() = default;
  TestStructArrayCustomData2(const TestStructArrayCustomData2 &rhs) = default;
  TestStructArrayCustomData2(TestStructArrayCustomData2 &&rhs) noexcept = default;
  ~TestStructArrayCustomData2() = default;
  bool operator==(const TestStructArrayCustomData2 &rhs) const;
  uint64_t GetF1() const;
  float GetF2() const;
  uint32_t GetF3() const;
  void SetF1(uint64_t val);
  void SetF2(float val);
  void SetF3(uint32_t val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructArrayCustomData2 *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructArrayCustomData2 ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructArrayCustomData2 &operator=(const TestStructArrayCustomData2 &rhs) = default;
  TestStructArrayCustomData2 &operator=(TestStructArrayCustomData2 &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructArrayCustomData2";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint64_t mF1 = 46;
  float mF2 = 47;
  uint32_t mF3 = 48;
};
}
