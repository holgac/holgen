// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestContainerInnerStructNoId {
public:
  bool operator==(const TestContainerInnerStructNoId& rhs) const;
  uint32_t GetField() const;
  const std::string& GetName() const;
  std::string& GetName();
  void SetField(uint32_t val);
  void SetName(const std::string& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static TestContainerInnerStructNoId* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  uint32_t mField;
  std::string mName;
};
}
