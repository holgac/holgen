// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructConstructor {
public:
  explicit TestStructConstructor(const float val);
  TestStructConstructor(const float x, const float y, const float z);
  TestStructConstructor() = default;
  TestStructConstructor(const TestStructConstructor &rhs) = default;
  TestStructConstructor(TestStructConstructor &&rhs) noexcept = default;
  ~TestStructConstructor() = default;
  bool operator==(const TestStructConstructor &rhs) const;
  float GetX() const;
  float GetY() const;
  float GetZ() const;
  void SetX(float val);
  void SetY(float val);
  void SetZ(float val);
  static TestStructConstructor Construct1(const float val);
  static TestStructConstructor Construct3(const float x, const float y, const float z);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructConstructor *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructConstructor ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructConstructor &operator=(const TestStructConstructor &rhs) = default;
  TestStructConstructor &operator=(TestStructConstructor &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructConstructor";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int Construct1CallerFromLua(lua_State *luaState);
  static int Construct3CallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  float mX;
  float mY;
  float mZ;
};
}
