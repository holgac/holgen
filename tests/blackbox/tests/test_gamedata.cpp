#include <gtest/gtest.h>
#include "GameData.h"
#include "JsonHelper.h"

using namespace holgen_blackbox_test;

TEST(GameDataTest, ElementGettersAndAdders) {
  // TODO: this tests tests everything, split a bit
  GameData gd;
  EXPECT_EQ(gd.GetArmors().size(), 0);
  {
    Armor a;
    a.SetName("plate mail");
    a.SetAlternativeName("platy");
    a.SetArmorClass(3);
    gd.AddArmor(std::move(a));
  }
  {
    Armor a;
    a.SetName("chain mail");
    a.SetAlternativeName("chainy");
    a.SetArmorClass(4);
    gd.AddArmor(std::move(a));
  }
  ASSERT_EQ(gd.GetArmors().size(), 2);
  EXPECT_EQ(gd.GetArmor(0).GetName(), "plate mail");
  EXPECT_EQ(gd.GetArmor(0).GetId(), 0);
  EXPECT_EQ(gd.GetArmor(1).GetName(), "chain mail");
  EXPECT_EQ(gd.GetArmor(1).GetId(), 1);

  auto a1 = gd.GetArmorFromName("chain mail");
  ASSERT_NE(a1, nullptr);
  EXPECT_EQ(a1->GetName(), "chain mail");
  EXPECT_EQ(a1->GetId(), 1);
  EXPECT_EQ(gd.GetArmorFromName("chain mail"), gd.GetArmorFromAlternativeName("chainy"));

  {
    Armor a;
    a.SetName("chain mail");
    a.SetArmorClass(4);
    EXPECT_FALSE(gd.AddArmor(std::move(a)));
  }

  auto a2 = gd.GetArmorFromName("chain mail2");
  EXPECT_EQ(a2, nullptr);
}
