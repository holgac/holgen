#include <gtest/gtest.h>
#include <map>
#include "Country.h"
#include "JsonHelper.h"
#include "Converter.h"

using namespace holgen_blackbox_test;

TEST(CountryTest, Converters) {
  rapidjson::Document doc;
  doc.Parse(R"DELIM(
{
"leader": {
  "race": "dwarf",
  "currentCountry": "Country1",
  "currentCity": "City1",
  "homeCountry": "Country0",
  "placeOfBirth": "City0"
},
"citizens": [
  {
    "race": "elf",
    "currentCountry": "Country1",
    "currentCity": "City2",
    "homeCountry": "Country3",
    "placeOfBirth": "City3"
  }
],
"population": {
  "dwarf": 17,
  "elf": 3
}
}
    )DELIM");
  std::map<std::string, uint32_t> cityMap = {
      {"City0", 0},
      {"City1", 1},
      {"City2", 2},
      {"City3", 3},
  };

  std::map<std::string, uint32_t> countryMap = {
      {"Country0", 0},
      {"Country1", 1},
      {"Country2", 2},
      {"Country3", 3},
  };
  std::map<std::string, uint32_t> raceMap = {
      {"elf",   0},
      {"dwarf", 1},
  };
  Country country;
  Converter converter;
  converter.raceNameToId = [&raceMap](const std::string &str) { return raceMap.at(str); };
  converter.countryNameToId = [&countryMap](const std::string &str) { return countryMap.at(str); };
  converter.cityNameToId = [&cityMap](const std::string &str) { return cityMap.at(str); };
  converter.raceU32Map = [&raceMap](const std::map<std::string, uint32_t> &input) {
    std::map<uint32_t, uint32_t> result;
    for (const auto&[raceStr, value]: input) {
      result.emplace(raceMap.at(raceStr), value);
    }
    return result;
  };

  JsonHelper::Parse(country, doc, converter);
  EXPECT_EQ(country.GetLeader().GetRace(), raceMap.at("dwarf"));
  EXPECT_EQ(country.GetLeader().GetCurrentCountry(), countryMap.at("Country1"));
  EXPECT_EQ(country.GetLeader().GetCurrentCity(), cityMap.at("City1"));
  EXPECT_EQ(country.GetLeader().GetHomeCountry(), countryMap.at("Country0"));
  EXPECT_EQ(country.GetLeader().GetPlaceOfBirth(), cityMap.at("City0"));
  auto &citizen = country.GetCitizens()[0];
  EXPECT_EQ(citizen.GetRace(), raceMap.at("elf"));
  EXPECT_EQ(citizen.GetCurrentCountry(), countryMap.at("Country1"));
  EXPECT_EQ(citizen.GetCurrentCity(), cityMap.at("City2"));
  EXPECT_EQ(citizen.GetHomeCountry(), countryMap.at("Country3"));
  EXPECT_EQ(citizen.GetPlaceOfBirth(), cityMap.at("City3"));
  EXPECT_EQ(country.GetPopulation().at(raceMap.at("dwarf")), 17);
  EXPECT_EQ(country.GetPopulation().at(raceMap.at("elf")), 3);
}

