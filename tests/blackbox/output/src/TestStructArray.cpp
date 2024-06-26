// This file is partially generated by holgen. Only modify the custom sections.
#include "../gen/TestStructArray.h"
// HOLGEN_USER_DEFINED_BEGIN:TestStructArray_CustomIncludes
#include "../gen/TestStructArrayCustomData1.h"
#include "../gen/TestStructArrayCustomData2.h"

// HOLGEN_USER_DEFINED_END:TestStructArray_CustomIncludes

namespace holgen_blackbox_test {
void TestStructArray::Initialize(const TestStructArrayType &type) {
// HOLGEN_USER_DEFINED_BEGIN:TestStructArray_Initialize
  mType = type;
  if (mType == TestStructArrayType::Type1)
    new (mCustomData.data()) TestStructArrayCustomData1();
  else if (mType == TestStructArrayType::Type2)
    new (mCustomData.data()) TestStructArrayCustomData2();
  else
    HOLGEN_FAIL("Invalid type: ", mType);
// HOLGEN_USER_DEFINED_END:TestStructArray_Initialize
}

TestStructArrayCustomData1 *TestStructArray::GetData1() {
// HOLGEN_USER_DEFINED_BEGIN:TestStructArray_GetData1
  HOLGEN_FAIL_IF(mType != TestStructArrayType::Type1, "Invalid type to get data1: ", mType);
  return reinterpret_cast<TestStructArrayCustomData1 *>(mCustomData.data());
// HOLGEN_USER_DEFINED_END:TestStructArray_GetData1
}

TestStructArrayCustomData2 *TestStructArray::GetData2() {
// HOLGEN_USER_DEFINED_BEGIN:TestStructArray_GetData2
  HOLGEN_FAIL_IF(mType != TestStructArrayType::Type2, "Invalid type to get data2: ", mType);
  return reinterpret_cast<TestStructArrayCustomData2 *>(mCustomData.data());
// HOLGEN_USER_DEFINED_END:TestStructArray_GetData2
}

}
