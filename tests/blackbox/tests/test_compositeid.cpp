#include <gtest/gtest.h>
#include <map>
#include "TestCompositeIdContainer.h"

using namespace holgen_blackbox_test;

namespace {
std::vector<TestCompositeIdHuman *> AddHumans(TestCompositeIdContainer &c, size_t count,
                                              size_t nameOffset = 0) {
  std::vector<TestCompositeIdHuman *> out;
  for (size_t i = 0; i < count; ++i) {
    TestCompositeIdHuman h;
    h.SetName(std::format("human{}", i + nameOffset));
    out.push_back(c.AddHuman(std::move(h)));
  }
  return out;
}
} // namespace

TEST(CompositeIdTest, AssignIdsForNewObjects) {
  TestCompositeIdContainer c;
  auto humans = AddHumans(c, 5);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(humans[i]->GetCompositeId(),
              TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, 0));
  }
}

TEST(CompositeIdTest, GetByCompositeId) {
  TestCompositeIdContainer c;
  auto humans = AddHumans(c, 5);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(humans[i],
              c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, 0)));
    EXPECT_EQ(nullptr,
              c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, 1)));
  }
}

TEST(CompositeIdTest, GetWithWrongVersion) {
  TestCompositeIdContainer c;
  auto humans = AddHumans(c, 5);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(nullptr,
              c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, 1)));
  }
}

TEST(CompositeIdTest, GetByIndex) {
  TestCompositeIdContainer c;
  auto humans = AddHumans(c, 5);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(humans[i], c.GetHumanFromName(std::format("human{}", i)));
  }
}

TEST(CompositeIdTest, Delete) {
  TestCompositeIdContainer c;
  AddHumans(c, 10);
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 2, 0));
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 5, 0));
  EXPECT_EQ(c.GetHumanCount(), 10);
  for (size_t i = 0; i < 10; ++i) {
    bool expectValid = i != 2 && i != 5;
    auto expectVersion = expectValid ? 0 : 1;
    EXPECT_EQ(c.GetHumanByRawIdx(i)->IsValid(), expectValid);
    EXPECT_EQ(c.GetHumanByRawIdx(i)->GetVersion(), expectVersion);
    auto ptr =
        c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, expectVersion));
    if (expectValid)
      EXPECT_NE(ptr, nullptr);
    else
      EXPECT_EQ(ptr, nullptr);
  }
}

TEST(CompositeIdTest, RecreateOne) {
  TestCompositeIdContainer c;
  AddHumans(c, 10);
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 2, 0));
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 5, 0));
  auto newHuman = AddHumans(c, 1, 10).front();
  EXPECT_EQ(c.GetHumanCount(), 10);
  EXPECT_TRUE(newHuman->GetId() == 2 || newHuman->GetId() == 5);
  auto deletedId = 7 - newHuman->GetId();

  for (size_t i = 0; i < 10; ++i) {
    bool expectValid = i != deletedId;
    auto expectVersion = i == 2 || i == 5 ? 1 : 0;
    EXPECT_EQ(c.GetHumanByRawIdx(i)->IsValid(), expectValid);
    EXPECT_EQ(c.GetHumanByRawIdx(i)->GetVersion(), expectVersion);
    auto ptr =
        c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, expectVersion));
    if (expectValid)
      EXPECT_NE(ptr, nullptr);
    else
      EXPECT_EQ(ptr, nullptr);
  }
}

TEST(CompositeIdTest, RecreateMany) {
  TestCompositeIdContainer c;
  AddHumans(c, 10);
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 2, 0));
  c.DeleteHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, 5, 0));
  AddHumans(c, 10, 10);
  EXPECT_EQ(c.GetHumanCount(), 18);
  for (size_t i = 0; i < 18; ++i) {
    bool expectValid = true;
    auto expectVersion = i == 2 || i == 5 ? 1 : 0;
    EXPECT_EQ(c.GetHumanByRawIdx(i)->IsValid(), expectValid);
    EXPECT_EQ(c.GetHumanByRawIdx(i)->GetVersion(), expectVersion);
    auto ptr =
        c.GetHuman(TestCompositeIdCompositeId(TestCompositeIdObjectType::Human, i, expectVersion));
    if (expectValid)
      EXPECT_NE(ptr, nullptr);
    else
      EXPECT_EQ(ptr, nullptr);
  }
}
