#include "DamageMultiplier.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
const std::string& DamageMultiplier::GetWhen() const {
  return mWhen;
}
std::string& DamageMultiplier::GetWhen() {
  return mWhen;
}
void DamageMultiplier::SetWhen(const std::string& val) {
  mWhen = val;
}
float DamageMultiplier::GetValue() const {
  return mValue;
}
void DamageMultiplier::SetValue(float val) {
  mValue = val;
}
bool DamageMultiplier::ParseJson(const rapidjson::Value& json) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "when")) {
      auto res = JsonHelper::Parse(mWhen, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "value")) {
      auto res = JsonHelper::Parse(mValue, data.value);
      if (!res)
        return false;
    }
  }
  return true;
}
}
