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
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructVirtualMethods {
public:
  TestStructVirtualMethods() = default;
  TestStructVirtualMethods(const TestStructVirtualMethods &rhs) = default;
  TestStructVirtualMethods(TestStructVirtualMethods &&rhs) noexcept = default;
  virtual ~TestStructVirtualMethods() = default;
  virtual uint32_t VirtualFunc();
  virtual uint32_t PureVirtualFunc() = 0;
  bool operator==(const TestStructVirtualMethods &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructVirtualMethods *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructVirtualMethods &operator=(const TestStructVirtualMethods &rhs) = default;
  TestStructVirtualMethods &operator=(TestStructVirtualMethods &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructVirtualMethods";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int VirtualFuncCallerFromLua(lua_State *luaState);
  static int PureVirtualFuncCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
};
}
