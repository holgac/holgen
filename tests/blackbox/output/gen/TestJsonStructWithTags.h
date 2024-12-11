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
// Defined in tests/blackbox/schemas/testjson.hsc
class TestJsonStructWithTags {
public:
  TestJsonStructWithTags() = default;
  TestJsonStructWithTags(const TestJsonStructWithTags &rhs) = default;
  TestJsonStructWithTags(TestJsonStructWithTags &&rhs) noexcept = default;
  ~TestJsonStructWithTags() = default;
  const std::vector<uint64_t> &GetTags() const;
  std::vector<uint64_t> &GetTags();
  void SetTags(const std::vector<uint64_t> &val);
  bool operator==(const TestJsonStructWithTags &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestJsonStructWithTags *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestJsonStructWithTags ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestJsonStructWithTags &operator=(const TestJsonStructWithTags &rhs) = default;
  TestJsonStructWithTags &operator=(TestJsonStructWithTags &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestJsonStructWithTags";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::vector<uint64_t> mTags;
};
}
