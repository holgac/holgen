#include <gtest/gtest.h>
#include "TestContainerVector.h"
#include "TestContainerMap.h"
#include "JsonHelper.h"
#include <format>

using namespace holgen_blackbox_test;

class ContainerTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
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
    ASSERT_NE(tcv.GetInnerStructWithIdFromName(name), nullptr);;
    EXPECT_EQ(tcv.GetInnerStructWithIdFromName(name)->GetId(), i);
  }
}

TEST_F(ContainerTest, NoIdVector) {
  TestContainerVector tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), i);
    EXPECT_EQ(tcv.GetInnerStructNoId(i), nullptr);
    TestContainerInnerStructNoId elem;
    auto name = std::format("test{}", i);
    elem.SetName(name);
    tcv.AddInnerStructNoId(std::move(elem));
    EXPECT_EQ(tcv.GetInnerStructNoIdCount(), i + 1);
    ASSERT_NE(tcv.GetInnerStructNoId(i), nullptr);
    EXPECT_EQ(tcv.GetInnerStructNoId(i)->GetName(), name);
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
    ASSERT_EQ(*tcv.GetStringElem(i), std::format("test{}", i));
  }
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
}

TEST_F(ContainerTest, WithIdMap) {
  TestContainerMap tcv;
  for (uint32_t i = 0; i < 5; i++) {
    EXPECT_EQ(tcv.GetInnerStructWithIdCount(), i);
    EXPECT_EQ(tcv.GetInnerStructWithId(i), nullptr);
    TestContainerInnerStructWithId elem;
    auto name = std::format("test{}", i);
    elem.SetName(name);
    tcv.AddInnerStructWithId(std::move(elem));
    EXPECT_EQ(tcv.GetInnerStructWithIdCount(), i + 1);
    ASSERT_NE(tcv.GetInnerStructWithId(i), nullptr);
    ASSERT_NE(tcv.GetInnerStructWithId(i), nullptr);
    EXPECT_EQ(tcv.GetInnerStructWithId(i)->GetName(), name);
    ASSERT_NE(tcv.GetInnerStructWithIdFromName(name), nullptr);;
    EXPECT_EQ(tcv.GetInnerStructWithIdFromName(name)->GetId(), i);
  }
}
