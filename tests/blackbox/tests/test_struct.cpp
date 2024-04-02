#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestStruct.h"
#include "JsonHelper.h"

using namespace holgen_blackbox_test;

class StructTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(StructTest, BasicTypes) {
  TestStruct testStruct;
  testStruct.SetTestFieldBool(true);
  EXPECT_EQ(testStruct.GetTestFieldBool(), true);
  testStruct.SetTestFieldBool(false);
  EXPECT_EQ(testStruct.GetTestFieldBool(), false);
  EXPECT_EQ( testStruct.GetTestFieldUnsigned(), 42);
  testStruct.SetTestFieldUnsigned(32);
  EXPECT_EQ( testStruct.GetTestFieldUnsigned(), 32);
  EXPECT_TRUE( testStruct.GetTestFieldString().empty());
  testStruct.SetTestFieldString("value");
  EXPECT_EQ( testStruct.GetTestFieldString(), "value");
}

TEST_F(StructTest, UserData) {
  TestStruct testStruct;
  std::string s;
  EXPECT_EQ(testStruct.GetTestFieldUserdata<std::string>(), nullptr);
  testStruct.SetTestFieldUserdata(&s);
  ASSERT_NE(testStruct.GetTestFieldUserdata<std::string>(), nullptr);
  EXPECT_TRUE(testStruct.GetTestFieldUserdata<std::string>()->empty());
  s = "value";
  EXPECT_EQ(*testStruct.GetTestFieldUserdata<std::string>(), "value");
}
