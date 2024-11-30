#include <gtest/gtest.h>
#include "TestContainerVector.h"
#include "TestContainerMap.h"
#include "TestContainerSet.h"
#include "JsonHelper.h"
#include "TestContainerEnum.h"
#include "TestContainerSkill.h"

#include <format>
#include <gmock/gmock-matchers.h>

using namespace holgen_blackbox_test;

class ContainerTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(ContainerTest, WithIdVector) {
  TestContainerVector tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetInnerStructWithIdCount(), i);
    EXPECT_EQ(tcv.GetInnerStructWithId(i), nullptr);
    TestContainerInnerStructWithId elem;
    auto name = std::format("test{}", i);
    elem.SetName(name);
    tcv.AddInnerStructWithId(std::move(elem));
    EXPECT_EQ(tcv.GetInnerStructWithIdCount(), i + 1);
    ASSERT_NE(tcv.GetInnerStructWithId(i), nullptr);
    EXPECT_EQ(tcv.GetInnerStructWithId(i)->GetName(), name);
    ASSERT_NE(tcv.GetInnerStructWithIdFromName(name), nullptr);
    EXPECT_EQ(tcv.GetInnerStructWithIdFromName(name)->GetId(), i);
  }
  // No deletion
}

TEST_F(ContainerTest, NoIdVector) {
  TestContainerVector tcv;
  for (uint32_t i = 0; i < 5; i++) {
    auto name = std::format("test{}", i);
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), i);
    EXPECT_EQ(tcv.GetInnerStructNoId(i), nullptr);
    EXPECT_EQ(tcv.GetInnerStructNoIdFromName(name), nullptr);
    TestContainerInnerStructNoId elem;
    elem.SetName(name);
    elem.SetField(i);
    tcv.AddInnerStructNoId(std::move(elem));
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), i + 1);
    ASSERT_NE(tcv.GetInnerStructNoId(i), nullptr);
    EXPECT_EQ(tcv.GetInnerStructNoId(i)->GetName(), name);
    ASSERT_NE(tcv.GetInnerStructNoIdFromName(name), nullptr);
    EXPECT_EQ(tcv.GetInnerStructNoIdFromName(name)->GetField(), i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    auto elem = tcv.GetInnerStructNoId(0);
    ASSERT_NE(elem, nullptr);
    auto name = elem->GetName();
    EXPECT_EQ(tcv.GetInnerStructNoIdFromName(name), elem);
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), 5 - i);
    tcv.DeleteInnerStructNoId(0);
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), 5 - i - 1);
    EXPECT_EQ(tcv.GetInnerStructNoIdFromName(name), nullptr);
  }
}

TEST_F(ContainerTest, StringVector) {
  TestContainerVector tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetStringElemCount(), i);
    EXPECT_EQ(tcv.GetStringElem(i), nullptr);
    tcv.AddStringElem(std::format("test{}", i));
    EXPECT_EQ(tcv.GetStringElemCount(), i + 1);
    ASSERT_NE(tcv.GetStringElem(i), nullptr);
    EXPECT_EQ(*tcv.GetStringElem(i), std::format("test{}", i));
  }
  EXPECT_THAT(tcv.GetStringContainer(),
              ::testing::UnorderedElementsAre("test0", "test1", "test2", "test3", "test4"));
  tcv.DeleteStringElem(4);
  EXPECT_THAT(tcv.GetStringContainer(),
              ::testing::UnorderedElementsAre("test0", "test1", "test2", "test3"));
  tcv.DeleteStringElem(1);
  EXPECT_THAT(tcv.GetStringContainer(), ::testing::UnorderedElementsAre("test0", "test2", "test3"));
}

TEST_F(ContainerTest, UnsignedVector) {
  TestContainerVector tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetUnsignedElemCount(), i);
    EXPECT_EQ(tcv.GetUnsignedElem(i), nullptr);
    tcv.AddUnsignedElem(i * 5);
    EXPECT_EQ(tcv.GetUnsignedElemCount(), i + 1);
    ASSERT_NE(tcv.GetUnsignedElem(i), nullptr);
    ASSERT_EQ(*tcv.GetUnsignedElem(i), i * 5);
  }
  EXPECT_THAT(tcv.GetUnsignedContainer(), ::testing::UnorderedElementsAre(0, 5, 10, 15, 20));
  tcv.DeleteUnsignedElem(4);
  EXPECT_THAT(tcv.GetUnsignedContainer(), ::testing::UnorderedElementsAre(0, 5, 10, 15));
  tcv.DeleteUnsignedElem(2);
  EXPECT_THAT(tcv.GetUnsignedContainer(), ::testing::UnorderedElementsAre(0, 5, 15));
}

TEST_F(ContainerTest, StringSet) {
  TestContainerSet tcv;
  for (uint32_t i = 0; i < 5; i++) {
    auto name = std::format("test{}", i);
    EXPECT_EQ(tcv.GetStringElemCount(), i);
    EXPECT_EQ(tcv.HasStringElem(name), false);
    tcv.AddStringElem(name);
    EXPECT_EQ(tcv.GetStringElemCount(), i + 1);
    EXPECT_EQ(tcv.HasStringElem(name), true);
  }
  for (uint32_t i = 0; i < 5; i++) {
    auto name = std::format("test{}", i);
    EXPECT_EQ(tcv.GetStringElemCount(), 5 - i);
    EXPECT_EQ(tcv.HasStringElem(name), true);
    tcv.DeleteStringElem(name);
    EXPECT_EQ(tcv.GetStringElemCount(), 5 - i - 1);
    EXPECT_EQ(tcv.HasStringElem(name), false);
  }
}

TEST_F(ContainerTest, UnsignedSet) {
  TestContainerSet tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetUnsignedElemCount(), i);
    EXPECT_EQ(tcv.HasUnsignedElem(i), false);
    tcv.AddUnsignedElem(i);
    EXPECT_EQ(tcv.GetUnsignedElemCount(), i + 1);
    EXPECT_EQ(tcv.HasUnsignedElem(i), true);
  }
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetUnsignedElemCount(), 5 - i);
    EXPECT_EQ(tcv.HasUnsignedElem(i), true);
    tcv.DeleteUnsignedElem(i);
    EXPECT_EQ(tcv.GetUnsignedElemCount(), 5 - i - 1);
    EXPECT_EQ(tcv.HasUnsignedElem(i), false);
  }
}

TEST_F(ContainerTest, EnumArray) {
  TestContainerEnum tce;
  tce.SetSkill(TestContainerSkill::Endurance, 10);
  EXPECT_EQ(tce.GetSkill(TestContainerSkill::Endurance), 10);
}

// TODO: test lua
// TODO: test json
