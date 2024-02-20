#pragma once

#include "pch.h"

namespace holgen_e2e_test {
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
