// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Number.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class Calculator {
public:
  Calculator() = default;
  Calculator(const Calculator &rhs) = default;
  Calculator(Calculator &&rhs) = default;
  ~Calculator() = default;
  bool operator==(const Calculator &rhs) const;
  const Number &GetCurVal() const;
  Number &GetCurVal();
  void SetCurVal(const Number &val);
  int64_t Add(lua_State *luaState, const int64_t val) const;
  void SetAddLuaFunc(std::string val);
  bool HasAddLuaFunc() const;
  Number *Subtract(lua_State *luaState, Number &val) const;
  void SetSubtractLuaFunc(std::string val);
  bool HasSubtractLuaFunc() const;
  int64_t SubtractThenMultiply(const int64_t lhs, const int64_t rhs);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Calculator *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Calculator ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Calculator &operator=(const Calculator &rhs) = default;
  Calculator &operator=(Calculator &&rhs) = default;
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int AddCallerFromLua(lua_State *luaState);
  static int SubtractCallerFromLua(lua_State *luaState);
  static int SubtractThenMultiplyCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  Number mCurVal;
  std::string mLuaFuncHandle_Add;
  std::string mLuaFuncHandle_Subtract;
};
}
