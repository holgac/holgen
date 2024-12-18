// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "WeaponType.h"
#include "WeaponTypeBow.h"
#include "WeaponTypeSword.h"

struct lua_State;
namespace ex3_schemas {
  class Converter;
}
namespace ex3_schemas {
// Defined in 3/schemas/character.hsc
class WeaponInInventory {
public:
  WeaponInInventory() = default;
  WeaponInInventory(const WeaponInInventory &rhs);
  WeaponInInventory(WeaponInInventory &&rhs) noexcept;
  ~WeaponInInventory();
  const WeaponTypeBow *GetWeaponAsWeaponTypeBow() const;
  WeaponTypeBow *GetWeaponAsWeaponTypeBow();
  const WeaponTypeSword *GetWeaponAsWeaponTypeSword() const;
  WeaponTypeSword *GetWeaponAsWeaponTypeSword();
  void SetType(const WeaponType &val);
  void ResetType();
  WeaponType GetType() const;
  bool operator==(const WeaponInInventory &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static WeaponInInventory *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static WeaponInInventory ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  WeaponInInventory &operator=(const WeaponInInventory &rhs);
  WeaponInInventory &operator=(WeaponInInventory &&rhs) noexcept;
  inline static const char *CLASS_NAME = "WeaponInInventory";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int ResetTypeCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  WeaponType mType;
  std::array<uint8_t, std::max({sizeof(WeaponTypeBow), sizeof(WeaponTypeSword)})> mWeapon;
};
}
