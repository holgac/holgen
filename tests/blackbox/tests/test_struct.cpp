#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestStruct.h"
#include "TestStructSingleElemContainer.h"
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
  EXPECT_EQ(testStruct.GetTestFieldUnsigned(), 42);
  testStruct.SetTestFieldUnsigned(32);
  EXPECT_EQ(testStruct.GetTestFieldUnsigned(), 32);
  EXPECT_TRUE(testStruct.GetTestFieldString().empty());
  testStruct.SetTestFieldString("value");
  EXPECT_EQ(testStruct.GetTestFieldString(), "value");
  EXPECT_EQ(testStruct, testStruct);
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

TEST_F(StructTest, SingleElem) {
  rapidjson::Document doc;
  doc.Parse(R"R(
{
  "singleElemStructs": [
    {"name": "elem1"},
    "elem2"
  ]
}
)R");
  TestStructSingleElemContainer container;
  container.ParseJson(doc, {});
  ASSERT_EQ(container.GetSingleElemStructCount(), 2);
  EXPECT_EQ(container.GetSingleElemStruct(0)->GetName(), "elem1");
  EXPECT_EQ(container.GetSingleElemStruct(1)->GetName(), "elem2");
}

TEST_F(StructTest, SingleElemWithId) {
  rapidjson::Document doc;
  doc.Parse(R"R(
{
  "singleElemStructsWithId": [
    {"name": "elem1"},
    "elem2"
  ]
}
  )R");
  TestStructSingleElemContainer container;
  container.ParseJson(doc, {});
  ASSERT_EQ(container.GetSingleElemStructWithIdCount(), 2);
  EXPECT_EQ(container.GetSingleElemStructWithId(0)->GetName(), "elem1");
  EXPECT_EQ(container.GetSingleElemStructWithId(1)->GetName(), "elem2");
}
