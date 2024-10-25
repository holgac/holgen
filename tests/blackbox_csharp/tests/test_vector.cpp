#include <gtest/gtest.h>
#include "Vector.h"

class VectorTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(VectorTest, MemberFunctions) {
  holgen_blackbox_csharp::Vector vec(2, 0, 2);
  EXPECT_FLOAT_EQ(vec.LengthSquared(), 8);
}