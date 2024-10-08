// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
  class TestLuaFuncTableContainer;
}
namespace holgen_blackbox_test {
class TestLuaFuncTableStatic {
public:
  TestLuaFuncTableStatic() = default;
  TestLuaFuncTableStatic(const TestLuaFuncTableStatic &rhs) = default;
  TestLuaFuncTableStatic(TestLuaFuncTableStatic &&rhs) = default;
  ~TestLuaFuncTableStatic() = default;
  bool operator==(const TestLuaFuncTableStatic &rhs) const;
  void SetTable(std::string val);
  const std::string &GetTable();
  void SetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const;
  int32_t GetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestLuaFuncTableStatic *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestLuaFuncTableStatic ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestLuaFuncTableStatic &operator=(const TestLuaFuncTableStatic &rhs) = default;
  TestLuaFuncTableStatic &operator=(TestLuaFuncTableStatic &&rhs) = default;
  inline static const char *CLASS_NAME = "TestLuaFuncTableStatic";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int SetFieldCallerFromLua(lua_State *luaState);
  static int GetFieldCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::string mTable;
};
}
