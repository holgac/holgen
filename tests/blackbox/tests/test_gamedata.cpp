#include <gtest/gtest.h>
#include "GameData.h"
#include "JsonHelper.h"
#include "GlobalPointer.h"
#include "Converter.h"

using namespace holgen_blackbox_test;

class GameDataTest: public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
    GlobalPointer<GameData>::SetInstance(nullptr);
  }
};

TEST_F(GameDataTest, ElementGettersAndAdders) {
  // TODO: this tests tests many things, split a bit
  GameData gd;
  EXPECT_EQ(gd.GetArmors().size(), 0);
  {
    Armor a;
    a.SetName("plate mail");
    a.SetAlternativeName("platy");
    a.SetArmorClass(3);
    EXPECT_NE(gd.AddArmor(std::move(a)), nullptr);
  }
  {
    Armor a;
    a.SetName("chain mail");
    a.SetAlternativeName("chainy");
    a.SetArmorClass(4);
    EXPECT_NE(gd.AddArmor(std::move(a)), nullptr);
  }
  ASSERT_EQ(gd.GetArmors().size(), 2);
  EXPECT_EQ(gd.GetArmor(0)->GetName(), "plate mail");
  EXPECT_EQ(gd.GetArmor(0)->GetId(), 0);
  EXPECT_EQ(gd.GetArmor(1)->GetName(), "chain mail");
  EXPECT_EQ(gd.GetArmor(1)->GetId(), 1);

  auto a1 = gd.GetArmorFromName("chain mail");
  ASSERT_NE(a1, nullptr);
  EXPECT_EQ(a1->GetName(), "chain mail");
  EXPECT_EQ(a1->GetId(), 1);
  EXPECT_EQ(gd.GetArmorFromName("chain mail"), gd.GetArmorFromAlternativeName("chainy"));

  {
    Armor a;
    a.SetName("chain mail");
    a.SetArmorClass(4);
    EXPECT_EQ(gd.AddArmor(std::move(a)), nullptr);
  }

  auto a2 = gd.GetArmorFromName("chain mail2");
  EXPECT_EQ(a2, nullptr);
}

TEST_F(GameDataTest, ParseFiles) {
  GameData gd;
  gd.ParseFiles("gamedata", {});
  auto plateMail = gd.GetArmorFromName("Plate Mail");
  ASSERT_NE(plateMail, nullptr);
  EXPECT_EQ(plateMail->GetArmorClass(), 3);
  auto wizardRobe = gd.GetArmorFromName("Wizard Robe");
  ASSERT_NE(wizardRobe, nullptr);
  auto gorion = gd.GetCharacterFromName("Gorion");
  ASSERT_NE(gorion, nullptr);
  EXPECT_EQ(gorion->GetArmorId(), wizardRobe->GetId());
}

TEST_F(GameDataTest, GlobalPointer) {
  GameData gd;
  GlobalPointer<GameData>::SetInstance(&gd);
  gd.ParseFiles("gamedata", {});
  auto plateMail = gd.GetArmorFromName("Plate Mail");
  ASSERT_NE(plateMail, nullptr);
  auto plateMailFromGlobalPointer = Armor::Get(plateMail->GetId());
  ASSERT_EQ(plateMail, plateMailFromGlobalPointer);
}

TEST_F(GameDataTest, Ref) {
  // TODO: move to fixture
  GameData gd;
  GlobalPointer<GameData>::SetInstance(&gd);
  gd.ParseFiles("gamedata", {});
  auto gorion = gd.GetCharacterFromName("Gorion");
  auto robe = gd.GetArmorFromName("Wizard Robe");
  ASSERT_NE(gorion, nullptr);
  ASSERT_NE(robe, nullptr);
  EXPECT_EQ(gorion->GetArmorId(), robe->GetId());
  EXPECT_EQ(gorion->GetArmor(), robe);
  EXPECT_EQ(robe, Armor::Get(robe->GetId()));

}
