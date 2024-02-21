#pragma once

#include <cstdint>
#include <vector>
#include <rapidjson/document.h>
#include "JsonHelper.h"

namespace holgen_blackbox_test {
class Weapon {
public:
  uint32_t GetDamageMin() const;
  void SetDamageMin(uint32_t val);
  uint32_t GetDamageMax() const;
  void SetDamageMax(uint32_t val);
  const std::vector<std::string>& GetModifiers() const;
  std::vector<std::string>& GetModifiers();
  void SetModifiers(const std::vector<std::string>& val);
  bool ParseJson(const rapidjson::Value& json);
protected:
private:
  uint32_t mDamageMin;
  uint32_t mDamageMax;
  std::vector<std::string> mModifiers;
};
}
