// This file is partially generated by holgen. Only modify the custom sections.
#include "../gen/TestStructComparable.h"
// HOLGEN_USER_DEFINED_BEGIN:TestStructComparable_CustomIncludes
// HOLGEN_USER_DEFINED_END:TestStructComparable_CustomIncludes

namespace holgen_blackbox_test {
bool TestStructComparable::operator<(const TestStructComparable &rhs) const {
// HOLGEN_USER_DEFINED_BEGIN:TestStructComparable_operator<_Const
  if (mField1 != rhs.mField1)
    return mField1 < rhs.mField1;
  return mField2 < rhs.mField2;
// HOLGEN_USER_DEFINED_END:TestStructComparable_operator<_Const
}

}
