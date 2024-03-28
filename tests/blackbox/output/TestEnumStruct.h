#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestEnum.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestEnumStruct {
public:
  const TestEnum& GetEnumField() const;
  TestEnum& GetEnumField();
  void SetEnumField(const TestEnum& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static TestEnumStruct* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  TestEnum mEnumField;
};
}