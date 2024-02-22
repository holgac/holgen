#include "Person.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
uint32_t Person::GetRace() const {
  return mRace;
}
void Person::SetRace(uint32_t val) {
  mRace = val;
}
uint32_t Person::GetCurrentCountry() const {
  return mCurrentCountry;
}
void Person::SetCurrentCountry(uint32_t val) {
  mCurrentCountry = val;
}
uint32_t Person::GetCurrentCity() const {
  return mCurrentCity;
}
void Person::SetCurrentCity(uint32_t val) {
  mCurrentCity = val;
}
uint32_t Person::GetHomeCountry() const {
  return mHomeCountry;
}
void Person::SetHomeCountry(uint32_t val) {
  mHomeCountry = val;
}
uint32_t Person::GetPlaceOfBirth() const {
  return mPlaceOfBirth;
}
void Person::SetPlaceOfBirth(uint32_t val) {
  mPlaceOfBirth = val;
}
bool Person::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "race")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mRace = converter.raceNameToId(temp);
    } else if (0 == strcmp(name, "currentCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp(name, "currentCity")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCity = converter.cityNameToId(temp);
    } else if (0 == strcmp(name, "homeCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mHomeCountry = converter.countryNameToId(temp);
    } else if (0 == strcmp(name, "placeOfBirth")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mPlaceOfBirth = converter.cityNameToId(temp);
    }
  }
  return true;
}
}
