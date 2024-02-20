#include "Weapon.h"

namespace holgen_e2e_test {
uint32_t Weapon::GetDamageMin() const {
  return mDamageMin;
}
void Weapon::SetDamageMin(uint32_t val) {
  mDamageMin = val;
}
uint32_t Weapon::GetDamageMax() const {
  return mDamageMax;
}
void Weapon::SetDamageMax(uint32_t val) {
  mDamageMax = val;
}
}
