#include <gtest/gtest.h>
#include "GameData.h"
#include "JsonHelper.h"

using namespace holgen_blackbox_test;

TEST(GameDataTest, ElementGettersAndAdders) {
  GameData gd;
  EXPECT_EQ(gd.GetArmors().size(), 0);
  {
    Armor a;
    a.SetName("plate mail");
    a.SetArmorClass(3);
    gd.AddArmor(std::move(a));
  }
  {
    Armor a;
    a.SetName("chain mail");
    a.SetArmorClass(4);
    gd.AddArmor(std::move(a));
  }
  EXPECT_EQ(gd.GetArmors().size(), 2);
  EXPECT_EQ(gd.GetArmor(0).GetName(), "plate mail");
  EXPECT_EQ(gd.GetArmor(0).GetId(), 0);
  EXPECT_EQ(gd.GetArmor(1).GetName(), "chain mail");
  EXPECT_EQ(gd.GetArmor(1).GetId(), 1);
}
