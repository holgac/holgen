#include "Country.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
const Person& Country::GetLeader() const {
  return mLeader;
}
Person& Country::GetLeader() {
  return mLeader;
}
void Country::SetLeader(const Person& val) {
  mLeader = val;
}
const std::vector<Person>& Country::GetCitizens() const {
  return mCitizens;
}
std::vector<Person>& Country::GetCitizens() {
  return mCitizens;
}
void Country::SetCitizens(const std::vector<Person>& val) {
  mCitizens = val;
}
const std::map<uint32_t, uint32_t>& Country::GetPopulation() const {
  return mPopulation;
}
std::map<uint32_t, uint32_t>& Country::GetPopulation() {
  return mPopulation;
}
void Country::SetPopulation(const std::map<uint32_t, uint32_t>& val) {
  mPopulation = val;
}
bool Country::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "leader")) {
      auto res = mLeader.ParseJson(data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "citizens")) {
      auto res = JsonHelper::Parse(mCitizens, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "population")) {
      std::map<std::string, uint32_t> temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mPopulation = std::move(converter.raceU32Map(temp));
    }
  }
  return true;
}
}
