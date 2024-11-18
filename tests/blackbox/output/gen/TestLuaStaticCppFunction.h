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
// Defined in tests/blackbox/schemas/testlua.hsc
class TestLuaStaticCppFunction {
public:
  TestLuaStaticCppFunction() = default;
  TestLuaStaticCppFunction(const TestLuaStaticCppFunction &rhs) = default;
  TestLuaStaticCppFunction(TestLuaStaticCppFunction &&rhs) noexcept = default;
  ~TestLuaStaticCppFunction() = default;
  uint32_t GetData() const;
  void SetData(uint32_t val);
  static TestLuaStaticCppFunction Factory(const uint32_t data);
  bool operator==(const TestLuaStaticCppFunction &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestLuaStaticCppFunction *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestLuaStaticCppFunction ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestLuaStaticCppFunction &operator=(const TestLuaStaticCppFunction &rhs) = default;
  TestLuaStaticCppFunction &operator=(TestLuaStaticCppFunction &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestLuaStaticCppFunction";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int FactoryCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mData;
};
}
