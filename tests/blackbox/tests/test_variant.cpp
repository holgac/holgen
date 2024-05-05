#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <rapidjson/document.h>
#include "TestVariantStructDifferentTypes.h"
#include "TestVariantStructSharedType.h"
#include "TestVariantStructExplicitType.h"

using namespace holgen_blackbox_test;

class VariantTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(VariantTest, DifferentTypes) {
  TestVariantStructDifferentTypes tvs;
  EXPECT_EQ(tvs.GetBeing1Type(), TestVariantStructType::Invalid);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);
  tvs.SetBeing1Type(TestVariantStructType::Human);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); });
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jean");
  tvs.GetBeing1AsTestVariantStructHuman()->SetName("Jeanne");
  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jeanne");

  tvs.SetBeing2Type(TestVariantStructType::Cat);
  EXPECT_EQ(tvs.GetBeing1Type(), TestVariantStructType::Human);
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructCat()->GetName(), "Whiskers");
  tvs.GetBeing2AsTestVariantStructCat()->SetName("Whisky");
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructCat()->GetName(), "Whisky");
}

TEST_F(VariantTest, SharedType) {
  TestVariantStructSharedType tvs;
  EXPECT_EQ(tvs.GetBeingType(), TestVariantStructType::Invalid);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);
  tvs.SetBeingType(TestVariantStructType::Human);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); });
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); });

  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jean");
  tvs.GetBeing1AsTestVariantStructHuman()->SetName("Jeanne");
  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jeanne");
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructHuman()->GetName(), "Jean");
}

TEST_F(VariantTest, ExplicitType) {
  TestVariantStructExplicitType tvs;
  EXPECT_EQ(tvs.GetType(), TestVariantStructType::Invalid);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);
  tvs.SetType(TestVariantStructType::Human);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); });
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); });

  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jean");
  tvs.GetBeing1AsTestVariantStructHuman()->SetName("Jeanne");
  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "Jeanne");
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructHuman()->GetName(), "Jean");
}

TEST_F(VariantTest, JsonSharedType) {
  TestVariantStructSharedType tvs;
  EXPECT_EQ(tvs.GetBeingType(), TestVariantStructType::Invalid);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);

  rapidjson::Document doc;
  doc.Parse(R"R({
  "beingType": "Human",
  "being1": {"name": "john", "nationality": "none"},
  "being2": {"name": "john2", "nationality": "non2e"}
})R");
  tvs.ParseJson(doc, {});
  EXPECT_EQ(tvs.GetBeingType(), TestVariantStructType::Human);

  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); });
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); });

  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructHuman()->GetName(), "john");
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructHuman()->GetName(), "john2");
}

TEST_F(VariantTest, JsonExplicitType) {
  TestVariantStructExplicitType tvs;
  EXPECT_EQ(tvs.GetType(), TestVariantStructType::Invalid);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructCat(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);

  rapidjson::Document doc;
  doc.Parse(R"R({
  "type": "Cat",
  "being1": {"name": "catty", "color": "orange"},
  "being2": {"name": "kitty", "color": "white"}
  })R");
  tvs.ParseJson(doc, {});
  EXPECT_EQ(tvs.GetType(), TestVariantStructType::Cat);

  EXPECT_THROW({ tvs.GetBeing1AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing1AsTestVariantStructCat(); });
  EXPECT_THROW({ tvs.GetBeing2AsTestVariantStructHuman(); }, std::runtime_error);
  EXPECT_NO_THROW({ tvs.GetBeing2AsTestVariantStructCat(); });

  EXPECT_EQ(tvs.GetBeing1AsTestVariantStructCat()->GetName(), "catty");
  EXPECT_EQ(tvs.GetBeing2AsTestVariantStructCat()->GetName(), "kitty");
}
