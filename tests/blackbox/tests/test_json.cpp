#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestJsonTagManager.h"
#include "TestJsonStructWithTags.h"
#include "TestJsonTag.h"
#include "JsonHelper.h"
#include "Converter.h"

using namespace holgen_blackbox_test;

class JsonTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(JsonTest, ElemConverter) {
  TestJsonTagManager tagManager;
  Converter cv;
  cv.testJsonConvertTag = [&](const std::string &tag) -> uint64_t {
    return tagManager.GetOrInsert(tag);
  };
  TestJsonStructWithTags testStruct1, testStruct2;

  EXPECT_EQ(tagManager.GetTagFromName("tag1"), nullptr);
  EXPECT_EQ(tagManager.GetTagFromName("tag2"), nullptr);
  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"tags": ["tag1", "tag2"]}
    )R");
    testStruct1.ParseJson(doc, cv);
  }
  ASSERT_NE(tagManager.GetTagFromName("tag1"), nullptr);
  EXPECT_EQ(tagManager.GetTagFromName("tag1")->GetId(), 0);
  ASSERT_NE(tagManager.GetTagFromName("tag2"), nullptr);
  EXPECT_EQ(tagManager.GetTagFromName("tag2")->GetId(), 1);
  EXPECT_THAT(testStruct1.GetTags(), ::testing::ElementsAre(0, 1));

  EXPECT_EQ(tagManager.GetTagFromName("tag3"), nullptr);
  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"tags": ["tag3", "tag2"]}
    )R");
    testStruct2.ParseJson(doc, cv);
  }
  ASSERT_NE(tagManager.GetTagFromName("tag3"), nullptr);
  EXPECT_EQ(tagManager.GetTagFromName("tag3")->GetId(), 2);
  EXPECT_THAT(testStruct2.GetTags(), ::testing::ElementsAre(2, 1));
}
