// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>

struct lua_State;
namespace ex3_schemas {
  class Converter;
}
namespace ex3_schemas {
class WeaponTypeSword {
public:
  WeaponTypeSword() = default;
  WeaponTypeSword(const WeaponTypeSword &rhs) = default;
  WeaponTypeSword(WeaponTypeSword &&rhs) = default;
  ~WeaponTypeSword() = default;
  bool operator==(const WeaponTypeSword &rhs) const;
  float GetSharpness() const;
  bool GetIsShortSword() const;
  void SetSharpness(float val);
  void SetIsShortSword(bool val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static WeaponTypeSword *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static WeaponTypeSword ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  WeaponTypeSword &operator=(const WeaponTypeSword &rhs) = default;
  WeaponTypeSword &operator=(WeaponTypeSword &&rhs) = default;
  inline static const char *CLASS_NAME = "WeaponTypeSword";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  float mSharpness;
  bool mIsShortSword;
};
}
