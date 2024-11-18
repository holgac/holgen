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
// Defined in tests/blackbox/schemas/testlua.hsc
class TestLuaFuncTable {
public:
  TestLuaFuncTable() = default;
  TestLuaFuncTable(const TestLuaFuncTable &rhs) = default;
  TestLuaFuncTable(TestLuaFuncTable &&rhs) noexcept = default;
  ~TestLuaFuncTable() = default;
  void SetTable(std::string val);
  const std::string &GetTable() const;
  void SetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const;
  int32_t GetField(lua_State *luaState, const TestLuaFuncTableContainer &container) const;
  bool operator==(const TestLuaFuncTable &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestLuaFuncTable *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestLuaFuncTable ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestLuaFuncTable &operator=(const TestLuaFuncTable &rhs) = default;
  TestLuaFuncTable &operator=(TestLuaFuncTable &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestLuaFuncTable";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int SetFieldCallerFromLua(lua_State *luaState);
  static int GetFieldCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::string mTable;
};
}
