#include "Damage.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
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
bool Damage::ParseJson(const rapidjson::Value& json) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "min")) {
      auto res = JsonHelper::Parse(mMin, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "max")) {
      auto res = JsonHelper::Parse(mMax, data.value);
      if (!res)
        return false;
    }
  }
  return true;
}
}
