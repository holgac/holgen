#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include <rapidjson/document.h>
#include "Person.h"
#include "Converter.h"

namespace holgen_blackbox_test {
class Country {
public:
  const Person& GetLeader() const;
  Person& GetLeader();
  void SetLeader(const Person& val);
  const std::vector<Person>& GetCitizens() const;
  std::vector<Person>& GetCitizens();
  void SetCitizens(const std::vector<Person>& val);
  const std::map<uint32_t, uint32_t>& GetPopulation() const;
  std::map<uint32_t, uint32_t>& GetPopulation();
  void SetPopulation(const std::map<uint32_t, uint32_t>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  Person mLeader;
  std::vector<Person> mCitizens;
  std::map<uint32_t, uint32_t> mPopulation;
};
}
