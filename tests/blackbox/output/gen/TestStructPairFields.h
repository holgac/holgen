// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <vector>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructPairFields {
public:
  TestStructPairFields() = default;
  TestStructPairFields(const TestStructPairFields &rhs) = default;
  TestStructPairFields(TestStructPairFields &&rhs) noexcept = default;
  ~TestStructPairFields() = default;
  const std::pair<int32_t, std::string> &GetIntStringPair() const;
  std::pair<int32_t, std::string> &GetIntStringPair();
  const std::vector<std::pair<std::string, int32_t>> &GetPairVector() const;
  std::vector<std::pair<std::string, int32_t>> &GetPairVector();
  void SetIntStringPair(const std::pair<int32_t, std::string> &val);
  void SetPairVector(const std::vector<std::pair<std::string, int32_t>> &val);
  bool operator==(const TestStructPairFields &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructPairFields *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructPairFields ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructPairFields &operator=(const TestStructPairFields &rhs) = default;
  TestStructPairFields &operator=(TestStructPairFields &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructPairFields";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::pair<int32_t, std::string> mIntStringPair;
  std::vector<std::pair<std::string, int32_t>> mPairVector;
};
}
