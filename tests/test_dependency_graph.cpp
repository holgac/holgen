#include <gtest/gtest.h>
#include "core/Exception.h"
#include "parser/DependencyGraph.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"

using namespace holgen;

namespace {}

class DependencyGraphTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}

  void ExpectProcessOrder(const std::string &schema, const std::vector<std::string> &expectedProcessOrder) {
    Tokenizer tokenizer(schema, "DependencyGraphTest");
    Parser parser;
    parser.Parse(tokenizer);
    DependencyGraph dg(parser.GetProject());
    auto &processOrder = dg.GetProcessOrder();
    ASSERT_EQ(processOrder.size(), expectedProcessOrder.size());
    for (size_t i = 0; i < processOrder.size(); ++i) {
      EXPECT_EQ(processOrder[i], expectedProcessOrder[i]);
    }
  }

  void ExpectErrorMessage(const std::string &schema, const std::string &expectedError) {
    Tokenizer tokenizer(schema, "DependencyGraphTest");
    Parser parser;
    parser.Parse(tokenizer);
    EXPECT_THROW(
        {
          try {
            DependencyGraph dg(parser.GetProject());
          } catch (Exception &exc) {
            std::string actualError = exc.what();
            actualError = actualError.substr(actualError.find(" ") + 1);
            EXPECT_EQ(actualError, expectedError);
            throw;
          }
        },
        Exception);
  }
};

TEST_F(DependencyGraphTest, CircularDependency) {
  ExpectErrorMessage(R"DELIM(
struct A {
  B b;
}
struct B {
  A a;
}
  )DELIM",
                     "Circular dependency detected between B and A");
}

TEST_F(DependencyGraphTest, LoadOrder) {
  ExpectProcessOrder(R"DELIM(
struct A {
  B b;
}
struct B {
  C c;
  D d;
}
struct C {
  u8 f;
}
struct D {
  C c;
}
  )DELIM",
                     {"C", "D", "B", "A"});
}

TEST_F(DependencyGraphTest, Refs) {
  ExpectProcessOrder(R"DELIM(
struct A {
  Ref<B> b;
}
struct B {
  Ref<A> a;
}
  )DELIM",
                     {"A", "B"});
}
