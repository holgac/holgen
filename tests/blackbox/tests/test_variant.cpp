#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestVariantStruct.h"

using namespace holgen_blackbox_test;

class VariantTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(VariantTest, Type1) {
  TestVariantStruct tvs;
  tvs.InitializeCustomDataAsType1();
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF1(), 43);
  EXPECT_FLOAT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF2(), 44);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF3(), 45);
  tvs.GetCustomDataAsTestVariantStructCustomData1()->SetF1(123);
  tvs.GetCustomDataAsTestVariantStructCustomData1()->SetF2(4.56);
  tvs.GetCustomDataAsTestVariantStructCustomData1()->SetF3(789);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF1(), 123);
  EXPECT_FLOAT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF2(), 4.56);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData1()->GetF3(), 789);
  EXPECT_THROW({ tvs.GetCustomDataAsTestVariantStructCustomData2(); }, std::runtime_error);
}

TEST_F(VariantTest, Type2) {
  TestVariantStruct tvs;
  tvs.InitializeCustomDataAsType2();
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF1(), 46);
  EXPECT_FLOAT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF2(), 47);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF3(), 48);
  tvs.GetCustomDataAsTestVariantStructCustomData2()->SetF1(123);
  tvs.GetCustomDataAsTestVariantStructCustomData2()->SetF2(4.56);
  tvs.GetCustomDataAsTestVariantStructCustomData2()->SetF3(789);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF1(), 123);
  EXPECT_FLOAT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF2(), 4.56);
  EXPECT_EQ(tvs.GetCustomDataAsTestVariantStructCustomData2()->GetF3(), 789);
  EXPECT_THROW({ tvs.GetCustomDataAsTestVariantStructCustomData1(); }, std::runtime_error);
}
