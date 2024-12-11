// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testvariant.hsc
class TestVariantStructCat {
public:
  TestVariantStructCat() = default;
  TestVariantStructCat(const TestVariantStructCat &rhs) = default;
  TestVariantStructCat(TestVariantStructCat &&rhs) noexcept = default;
  ~TestVariantStructCat() = default;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetColor() const;
  std::string &GetColor();
  void SetName(const std::string &val);
  void SetColor(const std::string &val);
  bool operator==(const TestVariantStructCat &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestVariantStructCat *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestVariantStructCat ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestVariantStructCat &operator=(const TestVariantStructCat &rhs) = default;
  TestVariantStructCat &operator=(TestVariantStructCat &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestVariantStructCat";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::string mName = "Whiskers";
  std::string mColor = "orange";
};
}
