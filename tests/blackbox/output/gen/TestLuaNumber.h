// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testlua.hsc
class TestLuaNumber {
public:
  TestLuaNumber() = default;
  TestLuaNumber(const TestLuaNumber &rhs) = default;
  TestLuaNumber(TestLuaNumber &&rhs) noexcept = default;
  ~TestLuaNumber() = default;
  int32_t GetValue() const;
  void SetValue(int32_t val);
  bool operator==(const TestLuaNumber &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestLuaNumber *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestLuaNumber ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestLuaNumber &operator=(const TestLuaNumber &rhs) = default;
  TestLuaNumber &operator=(TestLuaNumber &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestLuaNumber";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  int32_t mValue = 0;
};
}
