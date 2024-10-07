#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestStruct.h"
#include "TestStructSingleElemContainer.h"
#include "TestStructArray.h"
#include "JsonHelper.h"
#include "Converter.h"
#include "TestStructArrayCustomData1.h"
#include "TestStructArrayCustomData2.h"
#include "TestStructNonCopyable.h"
#include "TestStructHashable.h"
#include "TestStructHashableMap.h"
#include "TestStructComparable.h"
#include "TestStructComparableMap.h"
#include "TestStructPairFields.h"
#include "TestStructToString.h"

using namespace holgen_blackbox_test;

class StructTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
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
    ["elem2"]
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
    ["elem2"]
  ]
}
  )R");
  TestStructSingleElemContainer container;
  container.ParseJson(doc, {});
  ASSERT_EQ(container.GetSingleElemStructWithIdCount(), 2);
  EXPECT_EQ(container.GetSingleElemStructWithId(0)->GetName(), "elem1");
  EXPECT_EQ(container.GetSingleElemStructWithId(1)->GetName(), "elem2");
}

TEST_F(StructTest, ArrayCustomData1) {
  TestStructArray arr1;
  arr1.Initialize(TestStructArrayType::Type1);
  EXPECT_EQ(arr1.GetData1()->GetF1(), 43);
  EXPECT_FLOAT_EQ(arr1.GetData1()->GetF2(), 44);
  EXPECT_EQ(arr1.GetData1()->GetF3(), 45);
  arr1.GetData1()->SetF1(123);
  arr1.GetData1()->SetF2(4.56);
  arr1.GetData1()->SetF3(789);
  EXPECT_EQ(arr1.GetData1()->GetF1(), 123);
  EXPECT_FLOAT_EQ(arr1.GetData1()->GetF2(), 4.56);
  EXPECT_EQ(arr1.GetData1()->GetF3(), 789);
  EXPECT_THROW({ arr1.GetData2(); }, std::runtime_error);
}

TEST_F(StructTest, ArrayCustomData2) {
  TestStructArray arr1;
  arr1.Initialize(TestStructArrayType::Type2);
  EXPECT_EQ(arr1.GetData2()->GetF1(), 46);
  EXPECT_FLOAT_EQ(arr1.GetData2()->GetF2(), 47);
  EXPECT_EQ(arr1.GetData2()->GetF3(), 48);
  arr1.GetData2()->SetF1(123);
  arr1.GetData2()->SetF2(4.56);
  arr1.GetData2()->SetF3(789);
  EXPECT_EQ(arr1.GetData2()->GetF1(), 123);
  EXPECT_FLOAT_EQ(arr1.GetData2()->GetF2(), 4.56);
  EXPECT_EQ(arr1.GetData2()->GetF3(), 789);
  EXPECT_THROW({ arr1.GetData1(); }, std::runtime_error);
}

TEST_F(StructTest, NonCopyable) {
  TestStructNonCopyable a1;
  auto a2 = std::move(a1);
}

namespace {
template <typename MapType, typename DataType>
void TestMap() {
  MapType map;
  DataType data1;
  data1.SetField1(1);
  data1.SetField1(1001);
  map.GetData().emplace(data1, "data1");
  DataType data2;
  data2.SetField1(2);
  data2.SetField1(1002);
  map.GetData().emplace(data2, "data2");
  std::map<std::string, DataType> dataInMap;
  for (auto &[key, data]: map.GetData()) {
    dataInMap[data] = key;
  }

  EXPECT_EQ(map.GetData().size(), 2);
  ASSERT_TRUE(dataInMap.contains("data1"));
  ASSERT_TRUE(dataInMap.contains("data2"));
  EXPECT_EQ(dataInMap.at("data1"), data1);
  EXPECT_EQ(dataInMap.at("data2"), data2);

  ASSERT_TRUE(map.GetData().contains(data1));
  ASSERT_TRUE(map.GetData().contains(data2));
  EXPECT_EQ(map.GetData().at(data1), "data1");
  EXPECT_EQ(map.GetData().at(data2), "data2");
}
} // namespace

TEST_F(StructTest, Hashable) {
  TestMap<TestStructHashableMap, TestStructHashable>();
}

TEST_F(StructTest, Comparable) {
  TestMap<TestStructComparableMap, TestStructComparable>();
}

TEST_F(StructTest, PairFields) {
  TestStructPairFields data;
  data.SetIntStringPair(std::make_pair(5, std::string("hello")));
  EXPECT_EQ(data.GetIntStringPair().first, 5);
  EXPECT_EQ(data.GetIntStringPair().second, "hello");
}

TEST_F(StructTest, ToString) {
  TestStructToString data;
  data.SetField1("first");
  data.SetField2("second");
  EXPECT_EQ(std::format("{}", data), "first,second");
}
