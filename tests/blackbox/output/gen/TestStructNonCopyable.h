// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <vector>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructNonCopyable {
public:
  TestStructNonCopyable() = default;
  TestStructNonCopyable(const TestStructNonCopyable &rhs) = delete;
  TestStructNonCopyable(TestStructNonCopyable &&rhs) noexcept;
  ~TestStructNonCopyable() = default;
  const std::vector<int> &GetBigVector() const;
  std::vector<int> &GetBigVector();
  void SetBigVector(const std::vector<int> &val);
  bool operator==(const TestStructNonCopyable &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructNonCopyable *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructNonCopyable ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructNonCopyable &operator=(const TestStructNonCopyable &rhs) = delete;
  TestStructNonCopyable &operator=(TestStructNonCopyable &&rhs) noexcept;
  inline static const char *CLASS_NAME = "TestStructNonCopyable";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::vector<int> mBigVector;
};
}
