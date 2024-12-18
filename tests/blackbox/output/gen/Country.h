// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <vector>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "Person.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/person.hsc
class Country {
public:
  Country() = default;
  Country(const Country &rhs) = default;
  Country(Country &&rhs) noexcept = default;
  ~Country() = default;
  const Person &GetLeader() const;
  Person &GetLeader();
  const std::vector<Person> &GetCitizens() const;
  std::vector<Person> &GetCitizens();
  const std::map<uint32_t, uint32_t> &GetPopulation() const;
  std::map<uint32_t, uint32_t> &GetPopulation();
  void SetLeader(const Person &val);
  void SetCitizens(const std::vector<Person> &val);
  void SetPopulation(const std::map<uint32_t, uint32_t> &val);
  bool operator==(const Country &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Country *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Country ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Country &operator=(const Country &rhs) = default;
  Country &operator=(Country &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Country";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  Person mLeader;
  std::vector<Person> mCitizens;
  std::map<uint32_t, uint32_t> mPopulation;
};
}
