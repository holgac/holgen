// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/gamedata.hsc
class Armor {
public:
  using IdType=uint32_t;
  Armor() = default;
  Armor(const Armor &rhs) = default;
  Armor(Armor &&rhs) noexcept = default;
  ~Armor() = default;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetAlternativeName() const;
  std::string &GetAlternativeName();
  int8_t GetArmorClass() const;
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  void SetAlternativeName(const std::string &val);
  void SetArmorClass(int8_t val);
  static Armor *Get(uint32_t id);
  static Armor *GetFromName(const std::string &key);
  static Armor *GetFromAlternativeName(const std::string &key);
  bool operator==(const Armor &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Armor *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Armor ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Armor &operator=(const Armor &rhs) = default;
  Armor &operator=(Armor &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Armor";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  std::string mName;
  std::string mAlternativeName;
  int8_t mArmorClass;
};
}
