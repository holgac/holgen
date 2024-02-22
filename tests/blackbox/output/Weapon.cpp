#include "Weapon.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
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
const std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() const {
  return mDamageMultipliers;
}
std::vector<DamageMultiplier>& Weapon::GetDamageMultipliers() {
  return mDamageMultipliers;
}
void Weapon::SetDamageMultipliers(const std::vector<DamageMultiplier>& val) {
  mDamageMultipliers = val;
}
const std::vector<std::string>& Weapon::GetModifiers() const {
  return mModifiers;
}
std::vector<std::string>& Weapon::GetModifiers() {
  return mModifiers;
}
void Weapon::SetModifiers(const std::vector<std::string>& val) {
  mModifiers = val;
}
bool Weapon::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "damageMin")) {
      auto res = JsonHelper::Parse(mDamageMin, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "damageMax")) {
      auto res = JsonHelper::Parse(mDamageMax, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "damageMultipliers")) {
      auto res = JsonHelper::Parse(mDamageMultipliers, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "modifiers")) {
      auto res = JsonHelper::Parse(mModifiers, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
