#include "Person.h"

namespace holgen_e2e_test {
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
}
