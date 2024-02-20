#pragma once

#include <cstdint>

namespace holgen_blackbox_test {
class Damage {
public:
  uint32_t GetMin() const;
  void SetMin(uint32_t val);
  uint32_t GetMax() const;
  void SetMax(uint32_t val);
protected:
private:
  uint32_t mMin;
  uint32_t mMax;
};
}
