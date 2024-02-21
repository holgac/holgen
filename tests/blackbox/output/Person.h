#pragma once

#include <cstdint>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
class Person {
public:
  uint32_t GetAge() const;
  void SetAge(uint32_t val);
  float GetGender() const;
  void SetGender(float val);
  bool ParseJson(const rapidjson::Value& json);
protected:
private:
  uint32_t mAge;
  float mGender;
};
}
