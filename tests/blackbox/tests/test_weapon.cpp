#include <gtest/gtest.h>
#include "Weapon.h"

using namespace holgen_blackbox_test;

namespace {
  TEST(WeaponTest, SettersAndGetters) {
    Weapon weapon;
    weapon.SetDamageMin(15);
    weapon.SetDamageMax(30);
    EXPECT_EQ(weapon.GetDamageMin(), 15);
    EXPECT_EQ(weapon.GetDamageMax(), 30);
  }


  TEST(WeaponTest, ParseJson) {
    rapidjson::Document doc;
    Weapon weapon;
    JsonHelper::Parse(weapon, doc);
    // weapon.ParseJson(doc);
    EXPECT_EQ(weapon.GetDamageMin(), 15);
    EXPECT_EQ(weapon.GetDamageMax(), 30);
  }

}
