// This file is partially generated by holgen. Only modify the custom sections.
#include "../gen/SingletonCounter.h"
// HOLGEN_USER_DEFINED_BEGIN:SingletonCounter_CustomIncludes
// HOLGEN_USER_DEFINED_END:SingletonCounter_CustomIncludes

namespace holgen_blackbox_csharp {
uint32_t SingletonCounter::Get() const {
// HOLGEN_USER_DEFINED_BEGIN:SingletonCounter_Get_Const
  return mVal;
// HOLGEN_USER_DEFINED_END:SingletonCounter_Get_Const
}

void SingletonCounter::Bump(const uint32_t amount) {
// HOLGEN_USER_DEFINED_BEGIN:SingletonCounter_Bump
  mVal += amount;
// HOLGEN_USER_DEFINED_END:SingletonCounter_Bump
}

void SingletonCounter::Reset() {
// HOLGEN_USER_DEFINED_BEGIN:SingletonCounter_Reset
  mVal = 0;
// HOLGEN_USER_DEFINED_END:SingletonCounter_Reset
}

}