#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestJsonTagManager.h"
#include "TestJsonStructWithTags.h"
#include "TestJsonTag.h"
#include "JsonHelper.h"
#include "Converter.h"

#include <TestJsonStructMapWithConverters.h>

using namespace holgen_blackbox_test;

class JsonTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
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

TEST_F(JsonTest, MapConvertElem) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string& str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertElem": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv);
  }
  ASSERT_EQ(obj.GetTestMapConvertElem().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertElem().at("1"), 1);
  EXPECT_EQ(obj.GetTestMapConvertElem().at("2"), 2);
}

TEST_F(JsonTest, MapConvertKey) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string& str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertKey": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv);
  }
  ASSERT_EQ(obj.GetTestMapConvertKey().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertKey().at(1), "1");
  EXPECT_EQ(obj.GetTestMapConvertKey().at(2), "2");
}

TEST_F(JsonTest, MapConvertKeyElem) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string& str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertKeyElem": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv);
  }
  ASSERT_EQ(obj.GetTestMapConvertKeyElem().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertKeyElem().at(1), 1);
  EXPECT_EQ(obj.GetTestMapConvertKeyElem().at(2), 2);
}
