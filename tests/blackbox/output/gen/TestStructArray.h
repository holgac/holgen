// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestStructArrayType.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
  class TestStructArrayCustomData1;
  class TestStructArrayCustomData2;
}
namespace holgen_blackbox_test {
class TestStructArray {
public:
  bool operator==(const TestStructArray& rhs) const;
  const TestStructArrayType& GetType() const;
  TestStructArrayType& GetType();
  const std::array<uint8_t, 16>& GetCustomData() const;
  std::array<uint8_t, 16>& GetCustomData();
  void SetType(const TestStructArrayType& val);
  void SetCustomData(const std::array<uint8_t, 16>& val);
  void Initialize(const TestStructArrayType& type);
  TestStructArrayCustomData1* GetData1();
  TestStructArrayCustomData2* GetData2();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static TestStructArray* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  TestStructArrayType mType;
  std::array<uint8_t, 16> mCustomData;
};
}