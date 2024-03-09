#pragma once

#include "holgen.h"
#include <vector>
#include <map>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Person.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Country {
public:
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
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  Person mLeader;
  std::vector<Person> mCitizens;
  std::map<uint32_t, uint32_t> mPopulation;
};
}
