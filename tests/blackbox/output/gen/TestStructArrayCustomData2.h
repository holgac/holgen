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
class TestStructArrayCustomData2 {
public:
  bool operator==(const TestStructArrayCustomData2& rhs) const;
  uint64_t GetF1() const;
  float GetF2() const;
  uint32_t GetF3() const;
  void SetF1(uint64_t val);
  void SetF2(float val);
  void SetF3(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static TestStructArrayCustomData2* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  uint64_t mF1 = 46;
  float mF2 = 47;
  uint32_t mF3 = 48;
};
}