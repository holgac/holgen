#include <gtest/gtest.h>
#include "LandscapeType.h"
#include "GridCell.h"
#include "JsonHelper.h"
#include "GlobalPointer.h"

using namespace holgen_blackbox_test;

class EnumTest: public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
    // GlobalPointer<GameData>::SetInstance(nullptr);
  }
};

TEST_F(EnumTest, Operators) {
  auto lType = LandscapeType::Land;
  EXPECT_EQ(lType, LandscapeType::Land);
  EXPECT_EQ(lType, LandscapeType::LandValue);
  EXPECT_NE(lType, LandscapeType::Sea);
  EXPECT_NE(lType, LandscapeType::SeaValue);
}

TEST_F(EnumTest, JsonString) {
  GridCell gridCell;
  rapidjson::Document doc;
  doc.Parse(R"DELIM(
{
"x": 0,
"y": 2,
"landscape": "Sea"
}
  )DELIM");
  gridCell.ParseJson(doc, {});
  EXPECT_EQ(gridCell.GetLandscape(), LandscapeType::Sea);
}

TEST_F(EnumTest, JsonInt) {
  GridCell gridCell;
  rapidjson::Document doc;
  doc.Parse(R"DELIM(
{
"x": 0,
"y": 2,
"landscape": 1
}
  )DELIM");
  gridCell.ParseJson(doc, {});
  EXPECT_EQ(gridCell.GetLandscape(), LandscapeType(1));
}
