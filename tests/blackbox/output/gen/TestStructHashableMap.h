// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "TestStructHashable.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructHashableMap {
public:
  TestStructHashableMap() = default;
  TestStructHashableMap(const TestStructHashableMap &rhs) = default;
  TestStructHashableMap(TestStructHashableMap &&rhs) noexcept = default;
  ~TestStructHashableMap() = default;
  const std::unordered_map<TestStructHashable, std::string> &GetData() const;
  std::unordered_map<TestStructHashable, std::string> &GetData();
  void SetData(const std::unordered_map<TestStructHashable, std::string> &val);
  bool operator==(const TestStructHashableMap &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructHashableMap *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructHashableMap ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructHashableMap &operator=(const TestStructHashableMap &rhs) = default;
  TestStructHashableMap &operator=(TestStructHashableMap &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructHashableMap";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::unordered_map<TestStructHashable, std::string> mData;
};
}
