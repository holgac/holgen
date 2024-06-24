// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace ex2_schemas {
  class Converter;
}
namespace ex2_schemas {
class Weapon {
public:
  Weapon() = default;
  Weapon(const Weapon &rhs) = default;
  Weapon(Weapon &&rhs) = default;
  ~Weapon() = default;
  bool operator==(const Weapon &rhs) const;
  uint32_t GetId() const;
  uint8_t GetDamageMin() const;
  uint8_t GetDamageMax() const;
  void SetId(uint32_t val);
  uint8_t GetAverageDamage(lua_State *luaState) const;
  void SetGetAverageDamageLuaFunc(std::string val);
  bool HasGetAverageDamageLuaFunc() const;
  void Initialize();
  void SetDamage(const uint8_t min, const uint8_t max);
  uint8_t GetDamage();
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
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
  Weapon &operator=(Weapon &&rhs) = default;
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  uint8_t mDamageMin;
  uint8_t mDamageMax;
  std::string mLuaFuncHandle_GetAverageDamage;
};
}
