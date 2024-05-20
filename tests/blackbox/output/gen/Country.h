// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <vector>
#include <map>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Person.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class Country {
public:
  bool operator==(const Country& rhs) const;
  const Person& GetLeader() const;
  Person& GetLeader();
  const std::vector<Person>& GetCitizens() const;
  std::vector<Person>& GetCitizens();
  const std::map<uint32_t, uint32_t>& GetPopulation() const;
  std::map<uint32_t, uint32_t>& GetPopulation();
  void SetLeader(const Person& val);
  void SetCitizens(const std::vector<Person>& val);
  void SetPopulation(const std::map<uint32_t, uint32_t>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static Country* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  Person mLeader;
  std::vector<Person> mCitizens;
  std::map<uint32_t, uint32_t> mPopulation;
};
}