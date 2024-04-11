#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestEnum.h"
#include "TestEnumStruct.h"
#include "JsonHelper.h"

using namespace holgen_blackbox_test;

class EnumTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(EnumTest, Operators) {
  auto lType = TestEnum::Entry1;
  EXPECT_EQ(lType, TestEnum::Entry1);
  EXPECT_EQ(lType, TestEnum::Entry1Value);
  EXPECT_NE(lType, TestEnum::Entry5);
  EXPECT_NE(lType, TestEnum::Entry5Value);

  lType = TestEnum::Entry2Value;
  EXPECT_EQ(lType, TestEnum::Entry2);
}

TEST_F(EnumTest, JsonString) {
  TestEnumStruct testEnumStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({"enumField": "Entry5"})R");
  testEnumStruct.ParseJson(doc, {});
  EXPECT_EQ(testEnumStruct.GetEnumField(), TestEnum::Entry5);
}

TEST_F(EnumTest, JsonIntegral) {
  TestEnumStruct testEnumStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({"enumField": 5})R");
  testEnumStruct.ParseJson(doc, {});
  EXPECT_EQ(testEnumStruct.GetEnumField(), TestEnum::Entry5);
}

TEST_F(EnumTest, GetEntries) {
  EXPECT_THAT(TestEnum::GetEntries(), ::testing::ElementsAre(TestEnum::Entry5, TestEnum::Entry1, TestEnum::Entry2));
  EXPECT_THAT(TestEnum::GetEntryValues(), ::testing::ElementsAre(5, 0, 1));
}

TEST_F(EnumTest, StdUnorderedMap) {
  std::unordered_map<TestEnum, int> map;
  EXPECT_EQ(map.contains(TestEnum::Entry1), false);
  map[TestEnum::Entry1] = 1001;
  map[TestEnum::Entry5] = 1005;
  ASSERT_EQ(map.contains(TestEnum::Entry1), true);
  EXPECT_EQ(map.at(TestEnum::Entry1), 1001);
  ASSERT_EQ(map.contains(TestEnum::Entry5), true);
  EXPECT_EQ(map.at(TestEnum::Entry5), 1005);
  EXPECT_EQ(map.contains(TestEnum::Entry2), false);
}

TEST_F(EnumTest, StdMap) {
  std::map<TestEnum, int> map;
  EXPECT_EQ(map.contains(TestEnum::Entry1), false);
  map[TestEnum::Entry1] = 1001;
  map[TestEnum::Entry5] = 1005;
  ASSERT_EQ(map.contains(TestEnum::Entry1), true);
  EXPECT_EQ(map.at(TestEnum::Entry1), 1001);
  ASSERT_EQ(map.contains(TestEnum::Entry5), true);
  EXPECT_EQ(map.at(TestEnum::Entry5), 1005);
  EXPECT_EQ(map.contains(TestEnum::Entry2), false);
}
