// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class DamageMultiplier {
public:
  bool operator==(const DamageMultiplier& rhs) const;
  const std::string& GetWhen() const;
  std::string& GetWhen();
  float GetValue() const;
  void SetWhen(const std::string& val);
  void SetValue(float val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static DamageMultiplier* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  std::string mWhen;
  float mValue;
};
}
