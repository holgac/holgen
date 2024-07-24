#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestEnum.h"
#include "TestEnumStruct.h"
#include "TestEnumDefaultValue.h"
#include "JsonHelper.h"
#include "Converter.h"
#include "LuaTestHelper.h"


using namespace holgen_blackbox_test;

class EnumTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(EnumTest, Operators) {
  TestEnum lType = TestEnum::Entry1;
  EXPECT_EQ(lType, TestEnum::Entry1);
  EXPECT_EQ(lType.GetValue(), TestEnum::Entry1);
  EXPECT_NE(lType, TestEnum::Entry5);

  lType = TestEnum::Entry2;
  EXPECT_EQ(lType, TestEnum::Entry2);

  lType = 5;
  EXPECT_EQ(lType, TestEnum::Entry5);
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
  EXPECT_THAT(TestEnum::GetEntries(),
              ::testing::ElementsAre(TestEnum::Entry5, TestEnum::Entry1, TestEnum::Entry2));
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
  ASSERT_EQ(map.contains(TestEnum(5)), true);
  EXPECT_EQ(map.at(TestEnum(5)), 1005);
  EXPECT_EQ(map.contains(TestEnum::Entry2), false);
}

TEST_F(EnumTest, Formatter) {
  EXPECT_EQ(std::format("{}", TestEnum::Entry1), "Entry1");
  EXPECT_EQ(std::format("{}", TestEnum(TestEnum::Entry1)), "Entry1");
  EXPECT_EQ(std::format("{}", TestEnum(TestEnum::Entry2)), "Entry2");
  EXPECT_EQ(std::format("{}", TestEnum(TestEnum::Entry5)), "Entry5");
  EXPECT_EQ(std::format("{}", TestEnum(5)), "Entry5");
}

TEST_F(EnumTest, DefaultValue) {
  TestEnumDefaultValue enumDefaultValue;
  EXPECT_EQ(enumDefaultValue, TestEnumDefaultValue::DefaultEntry);
}

TEST_F(EnumTest, DefaultValueJson) {
  TestEnumStruct testEnumStruct;
  rapidjson::Document doc;
  doc.Parse(R"R({})R");
  testEnumStruct.ParseJson(doc, {});
  EXPECT_EQ(testEnumStruct.GetEnumDefaultValueField(), TestEnumDefaultValue::DefaultEntry);

  doc.Parse(R"R({"enumDefaultValueField": "some invalid data"})R");
  testEnumStruct.ParseJson(doc, {});
  EXPECT_EQ(testEnumStruct.GetEnumDefaultValueField(), TestEnumDefaultValue::Invalid);
}

TEST_F(EnumTest, DefaultValueLua) {
  LuaState luaContext;
  luaContext.Init();

  lua_newtable(luaContext);
  auto testEnumStruct = TestEnumStruct::ReadMirrorFromLua(luaContext, -1);
  EXPECT_EQ(testEnumStruct.GetEnumDefaultValueField(), TestEnumDefaultValue::DefaultEntry);
  lua_pop(luaContext, 1);

  lua_newtable(luaContext);
  lua_pushstring(luaContext, "enumDefaultValueField");
  lua_pushstring(luaContext, "some invalid data");
  lua_settable(luaContext, -3);
  testEnumStruct = TestEnumStruct::ReadMirrorFromLua(luaContext, -1);
  EXPECT_EQ(testEnumStruct.GetEnumDefaultValueField(), TestEnumDefaultValue::Invalid);
  lua_pop(luaContext, 1);
}
