// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <vector>
#include <string>
#include <rapidjson/fwd.h>
#include "DamageMultiplier.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class Weapon {
public:
  bool operator==(const Weapon &rhs) const;
  uint32_t GetDamageMin() const;
  uint32_t GetDamageMax() const;
  const std::vector<DamageMultiplier> &GetDamageMultipliers() const;
  std::vector<DamageMultiplier> &GetDamageMultipliers();
  const std::vector<std::string> &GetModifiers() const;
  std::vector<std::string> &GetModifiers();
  void SetDamageMin(uint32_t val);
  void SetDamageMax(uint32_t val);
  void SetDamageMultipliers(const std::vector<DamageMultiplier> &val);
  void SetModifiers(const std::vector<std::string> &val);
  uint32_t GetAverageDamage();
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static Weapon *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  uint32_t mDamageMin;
  uint32_t mDamageMax;
  std::vector<DamageMultiplier> mDamageMultipliers;
  std::vector<std::string> mModifiers;
};
}
