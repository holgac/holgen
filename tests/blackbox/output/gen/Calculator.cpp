// This file is generated by holgen. Do not modify manually.
#include "Calculator.h"

#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool Calculator::operator==(const Calculator &rhs) const {
  return
      mCurVal == rhs.mCurVal;
}

const Number &Calculator::GetCurVal() const {
  return mCurVal;
}

Number &Calculator::GetCurVal() {
  return mCurVal;
}

void Calculator::SetCurVal(const Number &val) {
  mCurVal = val;
}

int64_t Calculator::Add(lua_State *luaState, const int64_t val) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_Add.empty(), {}, "Calling unset Add function");
  lua_getglobal(luaState, mLuaFuncHandle_Add.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Add function {}", mLuaFuncHandle_Add);
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

void Calculator::SetAddLuaFunc(std::string val) {
  mLuaFuncHandle_Add = val;
}

bool Calculator::HasAddLuaFunc() const {
  return !mLuaFuncHandle_Add.empty();
}

Number *Calculator::Subtract(lua_State *luaState, Number &val) const {
  HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_Subtract.empty(), nullptr, "Calling unset Subtract function");
  lua_getglobal(luaState, "Ops");
  lua_pushstring(luaState, mLuaFuncHandle_Subtract.c_str());
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Subtract function Ops.{}", mLuaFuncHandle_Subtract);
    lua_pop(luaState, 1);
    return nullptr;
  }
  LuaHelper::Push(*this, luaState);
  LuaHelper::Push(val, luaState);
  lua_call(luaState, 2, 1);
  auto result = Number::ReadProxyFromLua(luaState, -1);
  lua_pop(luaState, 2);
  return result;
}

void Calculator::SetSubtractLuaFunc(std::string val) {
  mLuaFuncHandle_Subtract = val;
}

bool Calculator::HasSubtractLuaFunc() const {
  return !mLuaFuncHandle_Subtract.empty();
}

bool Calculator::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Calculator");
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
    if (0 == strcmp("curVal", name)) {
      auto res = JsonHelper::Parse(mCurVal, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Calculator.curVal field");
    } else if (0 == strcmp("Add", name)) {
      auto res = JsonHelper::Parse(mLuaFuncHandle_Add, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Calculator.Add");
    } else if (0 == strcmp("Subtract", name)) {
      auto res = JsonHelper::Parse(mLuaFuncHandle_Subtract, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Calculator.Subtract");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Calculator: {}", name);
    }
  }
  return true;
}

void Calculator::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "CalculatorMeta");
  lua_setmetatable(luaState, -2);
}

void Calculator::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "curVal");
  mCurVal.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
}

void Calculator::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Calculator *Calculator::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Calculator *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Calculator Calculator::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Calculator{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("curVal", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mCurVal = *Number::ReadProxyFromLua(luaState, -1);
      } else {
        result.mCurVal = Number::ReadMirrorFromLua(luaState, -1);
      }
    } else {
      HOLGEN_WARN("Unexpected lua field: Calculator.{}", key);
    }
    lua_pop(luaState, 1);
  }
  lua_pop(luaState, 1);
  return result;
}

int Calculator::IndexMetaMethod(lua_State *luaState) {
  auto instance = Calculator::ReadProxyFromLua(luaState, -2);
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("curVal", key)) {
    LuaHelper::Push(instance->mCurVal, luaState);
  } else if (0 == strcmp("Add", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = Calculator::ReadProxyFromLua(lsInner, -2);
      int64_t arg0;
      LuaHelper::Read(arg0, lsInner, -1);
      auto result = instance->Add(lsInner, arg0);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else if (0 == strcmp("Subtract", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = Calculator::ReadProxyFromLua(lsInner, -2);
      auto arg0 = Number::ReadProxyFromLua(lsInner, -1);
      auto result = instance->Subtract(lsInner, *arg0);
      result->PushToLua(lsInner);
      return 1;
    });
  } else if (0 == strcmp("SubtractThenMultiply", key)) {
    lua_pushcfunction(luaState, [](lua_State *lsInner) {
      auto instance = Calculator::ReadProxyFromLua(lsInner, -3);
      int64_t arg0;
      LuaHelper::Read(arg0, lsInner, -2);
      int64_t arg1;
      LuaHelper::Read(arg1, lsInner, -1);
      auto result = instance->SubtractThenMultiply(arg0, arg1);
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    HOLGEN_WARN("Unexpected lua field: Calculator.{}", key);
    return 0;
  }
  return 1;
}

int Calculator::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Calculator::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("curVal", key)) {
    LuaHelper::Read(instance->mCurVal, luaState, -1);
  } else {
    HOLGEN_WARN("Unexpected lua field: Calculator.{}", key);
  }
  return 0;
}

void Calculator::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Calculator::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Calculator::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "CalculatorMeta");
}
}
