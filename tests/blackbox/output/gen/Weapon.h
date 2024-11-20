// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <vector>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "DamageMultiplier.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/weapon.hsc
class Weapon {
public:
  Weapon() = default;
  Weapon(const Weapon &rhs) = default;
  Weapon(Weapon &&rhs) noexcept = default;
  ~Weapon() = default;
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
  bool operator==(const Weapon &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Weapon *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Weapon ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Weapon &operator=(const Weapon &rhs) = default;
  Weapon &operator=(Weapon &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Weapon";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int GetAverageDamageCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mDamageMin;
  uint32_t mDamageMax;
  std::vector<DamageMultiplier> mDamageMultipliers;
  std::vector<std::string> mModifiers;
};
}
