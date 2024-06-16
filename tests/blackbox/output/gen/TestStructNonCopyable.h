// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <vector>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestStructNonCopyable {
public:
  TestStructNonCopyable() = default;
  TestStructNonCopyable(TestStructNonCopyable &&rhs) = default;
  TestStructNonCopyable(const TestStructNonCopyable &rhs) = delete;
  ~TestStructNonCopyable() = default;
  TestStructNonCopyable &operator=(TestStructNonCopyable &&rhs) = default;
  TestStructNonCopyable &operator=(const TestStructNonCopyable &rhs) = delete;
  bool operator==(const TestStructNonCopyable &rhs) const;
  const std::vector<int> &GetBigVector() const;
  std::vector<int> &GetBigVector();
  void SetBigVector(const std::vector<int> &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
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
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::vector<int> mBigVector;
};
}
