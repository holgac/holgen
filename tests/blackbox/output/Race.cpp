#include "Race.h"

namespace holgen_blackbox_test {
uint32_t Race::GetId() const {
  return mId;
}
void Race::SetId(uint32_t val) {
  mId = val;
}
const std::vector<string>& Race::GetNames() const {
  return mNames;
}
std::vector<string>& Race::GetNames() {
  return mNames;
}
void Race::SetNames(std::vector<string> val) {
  mNames = val;
}
}
