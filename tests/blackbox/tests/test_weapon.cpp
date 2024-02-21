#include <gtest/gtest.h>
#include "Weapon.h"
#include "JsonHelper.h"

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
    doc.Parse(R"DELIM(
{
"damageMin": 25,
"damageMax": 35,
"modifiers": ["shining", "rusty"]
}
    )DELIM");
    Weapon weapon;
    JsonHelper::Parse(weapon, doc);
    EXPECT_EQ(weapon.GetDamageMin(), 25);
    EXPECT_EQ(weapon.GetDamageMax(), 35);
    EXPECT_EQ(weapon.GetModifiers().size(), 2);
    EXPECT_EQ(weapon.GetModifiers()[0], "shining");
    EXPECT_EQ(weapon.GetModifiers()[1], "rusty");
  }

}
