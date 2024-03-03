#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <rapidjson/fwd.h>
#include "DamageMultiplier.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Weapon {
public:
  uint32_t GetDamageMin() const;
  void SetDamageMin(uint32_t val);
  uint32_t GetDamageMax() const;
  void SetDamageMax(uint32_t val);
  const std::vector<DamageMultiplier>& GetDamageMultipliers() const;
  std::vector<DamageMultiplier>& GetDamageMultipliers();
  void SetDamageMultipliers(const std::vector<DamageMultiplier>& val);
  const std::vector<std::string>& GetModifiers() const;
  std::vector<std::string>& GetModifiers();
  void SetModifiers(const std::vector<std::string>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mDamageMin;
  uint32_t mDamageMax;
  std::vector<DamageMultiplier> mDamageMultipliers;
  std::vector<std::string> mModifiers;
};
}
