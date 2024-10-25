#include <gtest/gtest.h>
#include "Vector.h"
#include "DotNetHost.h"

class ModuleTest : public ::testing::Test {
protected:
  void SetUp() override {
    mDotNetHost.Start();
  }

  void TearDown() override {}
  DotNetHost mDotNetHost;
};

TEST_F(ModuleTest, MemberFunctions) {
  holgen_blackbox_csharp::Vector vec(2, 0, 2);
  EXPECT_FLOAT_EQ(vec.LengthSquared(), 8);
}