#include "Person.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
uint32_t Person::GetAge() const {
  return mAge;
}
void Person::SetAge(uint32_t val) {
  mAge = val;
}
float Person::GetGender() const {
  return mGender;
}
void Person::SetGender(float val) {
  mGender = val;
}
bool Person::ParseJson(const rapidjson::Value& json) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "age")) {
      auto res = JsonHelper::Parse(mAge, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "gender")) {
      auto res = JsonHelper::Parse(mGender, data.value);
      if (!res)
        return false;
    }
  }
  return true;
}
}
