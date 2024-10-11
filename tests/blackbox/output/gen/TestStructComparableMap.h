// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <rapidjson/fwd.h>
#include "TestStructComparable.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructComparableMap {
public:
  TestStructComparableMap() = default;
  TestStructComparableMap(const TestStructComparableMap &rhs) = default;
  TestStructComparableMap(TestStructComparableMap &&rhs) = default;
  ~TestStructComparableMap() = default;
  bool operator==(const TestStructComparableMap &rhs) const;
  const std::map<TestStructComparable, std::string> &GetData() const;
  std::map<TestStructComparable, std::string> &GetData();
  void SetData(const std::map<TestStructComparable, std::string> &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructComparableMap *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructComparableMap ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructComparableMap &operator=(const TestStructComparableMap &rhs) = default;
  TestStructComparableMap &operator=(TestStructComparableMap &&rhs) = default;
  inline static const char *CLASS_NAME = "TestStructComparableMap";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::map<TestStructComparable, std::string> mData;
};
}
