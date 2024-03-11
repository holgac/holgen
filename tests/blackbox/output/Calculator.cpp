#include "Calculator.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "LuaHelper.h"
#include "JsonHelper.h"

namespace holgen_blackbox_test {
const Number& Calculator::GetCurVal() const {
  return mCurVal;
}
Number& Calculator::GetCurVal() {
  return mCurVal;
}
void Calculator::SetCurVal(const Number& val) {
  mCurVal = val;
}
void Calculator::SetAddLuaFunc(std::string val) {
  mFuncName_add = val;
}
int64_t Calculator::Add(lua_State* luaState, int64_t val) const {
  HOLGEN_WARN_AND_RETURN_IF(mFuncName_add.empty(), {}, "Calling unset add function");
  lua_getglobal(luaState, mFuncName_add.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined add function {}", mFuncName_add);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push(*this, luaState);
  LuaHelper::Push(val, luaState);
  lua_call(luaState, 2, 1);
  int64_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}
void Calculator::SetSubtractLuaFunc(std::string val) {
  mFuncName_subtract = val;
}
Number* Calculator::Subtract(lua_State* luaState, const Number* val) const {
  HOLGEN_WARN_AND_RETURN_IF(mFuncName_subtract.empty(), {}, "Calling unset subtract function");
  lua_getglobal(luaState, "Ops");
  lua_pushstring(luaState, mFuncName_subtract.c_str());
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined subtract function Ops.{}", mFuncName_subtract);
    lua_pop(luaState, 1);
    return {};
  }
  LuaHelper::Push(*this, luaState);
  LuaHelper::Push(val, luaState);
  lua_call(luaState, 2, 1);
  Number* result;
  result = Number::ReadFromLua(luaState, -1);
  lua_pop(luaState, 2);
  return result;
}
bool Calculator::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "curVal")) {
      auto res = mCurVal.ParseJson(data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "add")) {
      std::string val;
      JsonHelper::Parse(val, data.value, converter);
      mFuncName_add = std::move(val);
    } else if (0 == strcmp(name, "subtract")) {
      std::string val;
      JsonHelper::Parse(val, data.value, converter);
      mFuncName_subtract = std::move(val);
    }
  }
  return true;
}
void Calculator::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CalculatorMeta");
  lua_setmetatable(luaState, -2);
}
Calculator* Calculator::ReadFromLua(lua_State* luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Calculator*)lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}
void Calculator::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Calculator::ReadFromLua(ls, -2);
    const char* key = lua_tostring(ls, -1);
    if (0 == strcmp("curVal", key)) {
      LuaHelper::Push(instance->mCurVal, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    auto instance = Calculator::ReadFromLua(ls, -3);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("curVal", key)) {
      LuaHelper::Read(instance->mCurVal, ls, -1);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CalculatorMeta");
}
}
