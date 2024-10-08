// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace ex3_schemas {
  class Converter;
}
namespace ex3_schemas {
class WeaponTypeBow {
public:
  WeaponTypeBow() = default;
  WeaponTypeBow(const WeaponTypeBow &rhs) = default;
  WeaponTypeBow(WeaponTypeBow &&rhs) = default;
  ~WeaponTypeBow() = default;
  bool operator==(const WeaponTypeBow &rhs) const;
  uint32_t GetRange() const;
  const std::string &GetMaterial() const;
  std::string &GetMaterial();
  void SetRange(uint32_t val);
  void SetMaterial(const std::string &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static WeaponTypeBow *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static WeaponTypeBow ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  WeaponTypeBow &operator=(const WeaponTypeBow &rhs) = default;
  WeaponTypeBow &operator=(WeaponTypeBow &&rhs) = default;
  inline static const char *CLASS_NAME = "WeaponTypeBow";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mRange;
  std::string mMaterial;
};
}
