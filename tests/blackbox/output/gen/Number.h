// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/lua.hsc
class Number {
public:
  Number() = default;
  Number(const Number &rhs) = default;
  Number(Number &&rhs) noexcept = default;
  ~Number() = default;
  int64_t GetValue() const;
  void SetValue(int64_t val);
  bool operator==(const Number &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Number *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Number ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Number &operator=(const Number &rhs) = default;
  Number &operator=(Number &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Number";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  int64_t mValue;
};
}
