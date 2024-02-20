#pragma once

#include "pch.h"

namespace holgen_e2e_test {
class Weapon {
public:
  uint32_t GetDamageMin() const;
  void SetDamageMin(uint32_t val);
  uint32_t GetDamageMax() const;
  void SetDamageMax(uint32_t val);
protected:
private:
  uint32_t mDamageMin;
  uint32_t mDamageMax;
};
}
