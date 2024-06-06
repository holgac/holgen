#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestBitmap.h"
#include "TestBitmapStruct.h"
#include "JsonHelper.h"
#include "Converter.h"

using namespace holgen_blackbox_test;

class BitmapTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(BitmapTest, Operators) {
  TestBitmap lType{TestBitmap::Entry1};
  EXPECT_EQ(lType, TestBitmap::Entry1);
  EXPECT_EQ(lType.GetValue(), TestBitmap::Entry1);
  EXPECT_NE(lType, TestBitmap::Entry5);

  lType = TestBitmap::Entry2;
  EXPECT_EQ(lType, TestBitmap::Entry2);

  lType = 32;
  EXPECT_EQ(lType, TestBitmap::Entry5);
}

TEST_F(BitmapTest, JsonString) {
  TestBitmapStruct testBitmapStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({"bitmapField": "Entry5"})R");
  testBitmapStruct.ParseJson(doc, {});
  EXPECT_EQ(testBitmapStruct.GetBitmapField(), TestBitmap::Entry5);
}

TEST_F(BitmapTest, JsonStringMultiple) {
  TestBitmapStruct testBitmapStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({"bitmapField": "Entry5,Entry1,Entry1,Entry1"})R");
  testBitmapStruct.ParseJson(doc, {});
  EXPECT_TRUE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry5));
  EXPECT_TRUE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry1));
  EXPECT_FALSE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry2));
}

TEST_F(BitmapTest, JsonIntegral) {
  TestBitmapStruct testBitmapStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({"bitmapField": 33})R");
  testBitmapStruct.ParseJson(doc, {});
  EXPECT_TRUE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry5));
  EXPECT_TRUE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry1));
  EXPECT_FALSE(testBitmapStruct.GetBitmapField().Has(TestBitmap::Entry2));
}

TEST_F(BitmapTest, GetEntries) {
  EXPECT_THAT(TestBitmap::GetEntries(),
              ::testing::ElementsAre(TestBitmap::Entry5, TestBitmap::Entry1, TestBitmap::Entry2, TestBitmap::Entry3));
}

TEST_F(BitmapTest, Formatter) {
  EXPECT_EQ(std::format("{}", TestBitmap::Entry1), "Entry1");
  EXPECT_EQ(std::format("{}", TestBitmap(TestBitmap::Entry1)), "Entry1");
  EXPECT_EQ(std::format("{}", TestBitmap(TestBitmap::Entry2)), "Entry2");
  EXPECT_EQ(std::format("{}", TestBitmap(TestBitmap::Entry5)), "Entry5");
  EXPECT_EQ(std::format("{}", TestBitmap(32)), "Entry5");
}

TEST_F(BitmapTest, FormatterMultiple) {
  TestBitmap tb;
  EXPECT_EQ(std::format("{}", tb), "");
  tb |= TestBitmap::Entry1;
  EXPECT_EQ(std::format("{}", tb), "Entry1");
  tb |= TestBitmap::Entry5;
  EXPECT_EQ(std::format("{}", tb), "Entry5,Entry1");
  EXPECT_EQ(std::format("{}", TestBitmap(3)), "Entry1,Entry2");
}
