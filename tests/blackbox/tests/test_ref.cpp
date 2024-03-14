#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include "HumanManager.h"

using namespace holgen_blackbox_test;

namespace {
}

class RefTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(RefTest, TestAddToContainer) {
  HumanManager humanManager;
  Human alice;
  alice.SetName("Alice");
  humanManager.AddHuman(std::move(alice));
  Human bob;
  bob.SetName("Bob");
  humanManager.AddHuman(std::move(bob));
  EXPECT_EQ(humanManager.GetHumanFromName("Bob")->GetId(), humanManager.GetHumanFromName("Alice")->GetId() + 1);
}
