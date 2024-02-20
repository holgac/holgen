#include "Damage.h"

namespace holgen_e2e_test {
uint32_t Damage::GetMin() const {
  return mMin;
}
void Damage::SetMin(uint32_t val) {
  mMin = val;
}
uint32_t Damage::GetMax() const {
  return mMax;
}
void Damage::SetMax(uint32_t val) {
  mMax = val;
}
}
