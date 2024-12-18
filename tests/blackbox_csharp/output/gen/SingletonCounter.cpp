// This file is generated by holgen. Do not modify manually.
#include "SingletonCounter.h"

namespace holgen_blackbox_csharp {
uint32_t SingletonCounter::GetVal() const {
  return mVal;
}

void SingletonCounter::SetVal(uint32_t val) {
  mVal = val;
}

bool SingletonCounter::operator==(const SingletonCounter &rhs) const {
  return !(
      mVal != rhs.mVal
  );
}
}
extern "C" {
  uint32_t holgen_blackbox_csharp_SingletonCounter_Get() {
    return holgen_blackbox_csharp::SingletonCounter::GetInstance().Get();
  }
  void holgen_blackbox_csharp_SingletonCounter_Bump(const uint32_t amount) {
    holgen_blackbox_csharp::SingletonCounter::GetInstance().Bump(amount);
  }
  void holgen_blackbox_csharp_SingletonCounter_Reset() {
    holgen_blackbox_csharp::SingletonCounter::GetInstance().Reset();
  }
}
