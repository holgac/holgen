#pragma once

#include <cstdint>


namespace holgen_blackbox_test {
class Person {
public:
  uint32_t GetAge() const;
  void SetAge(uint32_t val);
  float GetGender() const;
  void SetGender(float val);
protected:
private:
  uint32_t mAge;
  float mGender;
};
}
