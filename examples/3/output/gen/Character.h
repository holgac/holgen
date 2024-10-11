// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <vector>
#include <rapidjson/fwd.h>
#include "Race.h"
#include "WeaponInInventory.h"

struct lua_State;
namespace ex3_schemas {
  class Converter;
}
namespace ex3_schemas {
// Defined in 3/schemas/character.hsc
class Character {
public:
  Character() = default;
  Character(const Character &rhs) = default;
  Character(Character &&rhs) = default;
  ~Character() = default;
  bool operator==(const Character &rhs) const;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  const Race &GetRace() const;
  Race &GetRace();
  const std::vector<WeaponInInventory> &GetWeapons() const;
  std::vector<WeaponInInventory> &GetWeapons();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  void SetRace(const Race &val);
  void SetWeapons(const std::vector<WeaponInInventory> &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Character *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Character ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Character &operator=(const Character &rhs) = default;
  Character &operator=(Character &&rhs) = default;
  inline static const char *CLASS_NAME = "Character";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  std::string mName;
  Race mRace;
  std::vector<WeaponInInventory> mWeapons;
};
}
