#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestJsonTagManager.h"
#include "TestJsonStructWithTags.h"
#include "TestJsonTag.h"
#include "JsonHelper.h"
#include "Converter.h"
#include "TestStructPairFields.h"
#include "TestJsonStructMapWithConverters.h"
#include "TestJsonEnum.h"
#include "TestJsonStructWithSingleTag.h"
#include "TestJsonStructContainer.h"
#include "TestVariantStructExplicitType.h"

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
    testStruct1.ParseJson(doc, cv, nullptr);
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
    testStruct2.ParseJson(doc, cv, nullptr);
  }
  ASSERT_NE(tagManager.GetTagFromName("tag3"), nullptr);
  EXPECT_EQ(tagManager.GetTagFromName("tag3")->GetId(), 2);
  EXPECT_THAT(testStruct2.GetTags(), ::testing::ElementsAre(2, 1));
}

TEST_F(JsonTest, MapConvertElem) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string &str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertElem": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv, nullptr);
  }
  ASSERT_EQ(obj.GetTestMapConvertElem().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertElem().at("1"), 1);
  EXPECT_EQ(obj.GetTestMapConvertElem().at("2"), 2);
}

TEST_F(JsonTest, MapConvertKey) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string &str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertKey": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv, nullptr);
  }
  ASSERT_EQ(obj.GetTestMapConvertKey().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertKey().at(1), "1");
  EXPECT_EQ(obj.GetTestMapConvertKey().at(2), "2");
}

TEST_F(JsonTest, MapConvertKeyElem) {
  TestJsonStructMapWithConverters obj;
  auto cv = Converter{};
  cv.testJsonConvertStringToU32 = [](const std::string &str) {
    return uint32_t(atoll(str.c_str()));
  };

  {
    rapidjson::Document doc;
    doc.Parse(R"R(
{"testMapConvertKeyElem": {"1": "1", "2": "2"}}
    )R");
    obj.ParseJson(doc, cv, nullptr);
  }
  ASSERT_EQ(obj.GetTestMapConvertKeyElem().size(), 2);
  EXPECT_EQ(obj.GetTestMapConvertKeyElem().at(1), 1);
  EXPECT_EQ(obj.GetTestMapConvertKeyElem().at(2), 2);
}

TEST_F(JsonTest, ParsingPairFields) {
  TestStructPairFields data;
  rapidjson::Document doc;
  doc.Parse(R"R(
{
  "intStringPair": [5, "hello"],
  "pairVector": [
    ["hello", 5],
    ["howareyou", 9]
  ]
}
)R");
  data.ParseJson(doc, {}, nullptr);
  EXPECT_EQ(data.GetIntStringPair().first, 5);
  EXPECT_EQ(data.GetIntStringPair().second, "hello");
  ASSERT_EQ(data.GetPairVector().size(), 2);
  EXPECT_EQ(data.GetPairVector()[0].first, "hello");
  EXPECT_EQ(data.GetPairVector()[0].second, 5);
  EXPECT_EQ(data.GetPairVector()[1].first, "howareyou");
  EXPECT_EQ(data.GetPairVector()[1].second, 9);
}

TEST_F(JsonTest, DumpAndParseEnum) {
  rapidjson::Document doc;
  TestJsonEnum e = TestJsonEnum::Entry5;
  auto json = e.DumpJson(doc, nullptr);
  EXPECT_EQ(json.GetType(), rapidjson::kNumberType);
  TestJsonEnum e2;
  e2.ParseJson(json, {}, nullptr);
  EXPECT_EQ(e2, e);
}

TEST_F(JsonTest, DumpAndParseInvalidEnum) {
  rapidjson::Document doc;
  TestJsonEnum e;
  auto json = e.DumpJson(doc, nullptr);
  EXPECT_EQ(json.GetType(), rapidjson::kNumberType);
  TestJsonEnum e2;
  e2.ParseJson(json, {}, nullptr);
  EXPECT_EQ(e2, e);
  EXPECT_EQ(e2, TestJsonEnum::Invalid);
}

TEST_F(JsonTest, DumpAndParsePrimitiveStruct) {
  rapidjson::Document doc;
  TestJsonTag tag;
  tag.SetId(123);
  tag.SetName("tagName");
  auto json = tag.DumpJson(doc, nullptr);
  EXPECT_EQ(json.GetType(), rapidjson::kObjectType);
  ASSERT_NE(json.FindMember("id"), json.MemberEnd());
  ASSERT_EQ(json.FindMember("id")->value.GetType(), rapidjson::kNumberType);
  EXPECT_EQ(json.FindMember("id")->value.GetInt64(), 123);

  ASSERT_NE(json.FindMember("name"), json.MemberEnd());
  ASSERT_EQ(json.FindMember("name")->value.GetType(), rapidjson::kStringType);
  EXPECT_EQ(std::string(json.FindMember("name")->value.GetString()), "tagName");

  TestJsonTag tag2;
  tag2.ParseJson(json, {}, nullptr);
  EXPECT_EQ(tag, tag2);
}

TEST_F(JsonTest, DumpStructField) {
  rapidjson::Document doc;
  TestJsonStructWithSingleTag obj, obj2;
  obj.GetTag().SetId(123);
  obj.GetTag().SetName("tagName");
  auto json = obj.DumpJson(doc, nullptr);
  obj2.ParseJson(json, {}, nullptr);
  EXPECT_EQ(obj, obj2);
}

TEST_F(JsonTest, DumpVector) {
  TestJsonStructContainer obj, obj2;
  obj.GetTags().emplace_back(123, "asdf");
  obj.GetTags().emplace_back(456, "qwer");
  rapidjson::Document doc;
  obj2.ParseJson(obj.DumpJson(doc, nullptr), {}, nullptr);
  EXPECT_EQ(obj, obj2);
}

TEST_F(JsonTest, DumpMap) {
  TestJsonStructContainer obj, obj2;
  obj.GetTagMap().emplace("asdf", TestJsonTag{123, "qwer"});
  obj.GetTagMap().emplace("zxcv", TestJsonTag{456, "rewq"});
  rapidjson::Document doc;
  obj2.ParseJson(obj.DumpJson(doc, nullptr), {}, nullptr);
  EXPECT_EQ(obj, obj2);
}

TEST_F(JsonTest, Variants) {
  TestVariantStructExplicitType obj, obj2;
  rapidjson::Document doc;
  obj2.ParseJson(obj.DumpJson(doc, nullptr), {}, nullptr);
  EXPECT_EQ(obj, obj2);
  obj.SetType(TestVariantStructType::Human);
  obj.GetBeing1AsTestVariantStructHuman()->SetName("Urist");
  obj.GetBeing1AsTestVariantStructHuman()->SetNationality("Dorf");
  obj.GetBeing2AsTestVariantStructHuman()->SetName("McMiner");
  obj.GetBeing2AsTestVariantStructHuman()->SetNationality("Dorf");
  obj2.ParseJson(obj.DumpJson(doc, nullptr), {}, nullptr);
  EXPECT_EQ(obj, obj2);
}
