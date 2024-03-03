#pragma once

#include "holgen.h"
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class DamageMultiplier {
public:
  const std::string& GetWhen() const;
  std::string& GetWhen();
  void SetWhen(const std::string& val);
  float GetValue() const;
  void SetValue(float val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  std::string mWhen;
  float mValue;
};
}
