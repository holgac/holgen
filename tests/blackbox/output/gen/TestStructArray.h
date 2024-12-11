// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "TestStructArrayType.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
  class TestStructArrayCustomData1;
  class TestStructArrayCustomData2;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructArray {
public:
  TestStructArray() = default;
  TestStructArray(const TestStructArray &rhs) = default;
  TestStructArray(TestStructArray &&rhs) noexcept = default;
  ~TestStructArray() = default;
  const TestStructArrayType &GetType() const;
  TestStructArrayType &GetType();
  const std::array<uint8_t, 16> &GetCustomData() const;
  std::array<uint8_t, 16> &GetCustomData();
  void SetType(const TestStructArrayType &val);
  void SetCustomData(const std::array<uint8_t, 16> &val);
  void Initialize(const TestStructArrayType &type);
  TestStructArrayCustomData1 *GetData1();
  TestStructArrayCustomData2 *GetData2();
  bool operator==(const TestStructArray &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructArray *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructArray ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructArray &operator=(const TestStructArray &rhs) = default;
  TestStructArray &operator=(TestStructArray &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructArray";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int GetData1CallerFromLua(lua_State *luaState);
  static int GetData2CallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  TestStructArrayType mType;
  std::array<uint8_t, 16> mCustomData;
};
}
