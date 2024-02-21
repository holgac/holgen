#pragma once

#include <cstdint>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
class Damage {
public:
  uint32_t GetMin() const;
  void SetMin(uint32_t val);
  uint32_t GetMax() const;
  void SetMax(uint32_t val);
  bool ParseJson(const rapidjson::Value& json);
protected:
private:
  uint32_t mMin;
  uint32_t mMax;
};
}
