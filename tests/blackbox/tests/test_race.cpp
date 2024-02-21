#include <gtest/gtest.h>
#include "Race.h"

using namespace holgen_blackbox_test;

namespace {

  TEST(RaceTest, ParseNestedContainers) {
    rapidjson::Document doc;
    doc.Parse(R"DELIM(
{
"name": "human",
"hairColors": ["black", "blond", "ginger"],
"names": {"male": ["John", "Jean"], "female": ["Alice", "Nancy"]}
}
    )DELIM");
    Race race;
    JsonHelper::Parse(race, doc);
    EXPECT_EQ(race.GetName(), "human");
    EXPECT_EQ(race.GetHairColors().size(), 3);
    EXPECT_EQ(race.GetHairColors()[0], "black");
    EXPECT_EQ(race.GetHairColors()[1], "blond");
    EXPECT_EQ(race.GetHairColors()[2], "ginger");
    EXPECT_EQ(race.GetNames().size(), 2);
    EXPECT_EQ(race.GetNames().at("male").size(), 2);
    EXPECT_EQ(race.GetNames().at("male")[0], "John");
    EXPECT_EQ(race.GetNames().at("male")[1], "Jean");
    EXPECT_EQ(race.GetNames().at("female").size(), 2);
    EXPECT_EQ(race.GetNames().at("female")[0], "Alice");
    EXPECT_EQ(race.GetNames().at("female")[1], "Nancy");
  }

}
