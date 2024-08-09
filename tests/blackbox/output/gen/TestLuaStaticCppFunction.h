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
class TestLuaStaticCppFunction {
public:
  TestLuaStaticCppFunction() = default;
  TestLuaStaticCppFunction(const TestLuaStaticCppFunction &rhs) = default;
  TestLuaStaticCppFunction(TestLuaStaticCppFunction &&rhs) = default;
  ~TestLuaStaticCppFunction() = default;
  bool operator==(const TestLuaStaticCppFunction &rhs) const;
  uint32_t GetData() const;
  void SetData(uint32_t val);
  static TestLuaStaticCppFunction Factory(const uint32_t data);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
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
  TestLuaStaticCppFunction &operator=(TestLuaStaticCppFunction &&rhs) = default;
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int FactoryCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mData;
};
}
