#pragma once

#include "holgen.h"
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Number.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Calculator {
public:
  const Number& GetCurVal() const;
  Number& GetCurVal();
  void SetCurVal(const Number& val);
  void SetAddLuaFunc(std::string val);
  int64_t Add(lua_State* luaState, int64_t val) const;
  void SetSubtractLuaFunc(std::string val);
  Number* Subtract(lua_State* luaState, const Number* val) const;
  int64_t SubtractThenMultiply(int64_t lhs, int64_t rhs);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static Calculator* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  Number mCurVal;
  std::string mFuncName_Add;
  std::string mFuncName_Subtract;
};
}
