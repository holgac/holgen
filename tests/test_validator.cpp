#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "parser/MonolithValidator.h"
#include "core/Exception.h"

using namespace holgen;

namespace {
}

class ValidatorTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }

  void ExpectErrorMessage(const std::string &schema, const std::string &expectedError) {
    Tokenizer tokenizer(schema, "ValidatorTest");
    Parser parser;
    parser.Parse(tokenizer);
    MonolithValidator validator(parser.GetProject());
    EXPECT_THROW({
                   try {
                     validator.Validate();
                   } catch (Exception &exc) {
                     std::string actualError = exc.what();
                     actualError = actualError.substr(actualError.find(" ") + 1);
                     EXPECT_EQ(actualError, expectedError);
                     throw;
                   }
                 }, Exception);
  }
};

TEST_F(ValidatorTest, Id) {
  ExpectErrorMessage(R"DELIM(
struct Person {
  @id()
  u32 id;
  @id()
  u32 ssn;
}
  )DELIM", "struct Person has multiple id fields: id and ssn");

  ExpectErrorMessage(R"DELIM(
struct Person {
  @id()
  vector<u8> field;
}

  )DELIM", "Field Person.field uses an invalid type for an id: vector");
  ExpectErrorMessage(R"DELIM(
struct Attributes {
  u8 strength;
  u8 dexterity;
}
struct Person {
  @id()
  Attributes attributes;
}

  )DELIM", "Field Person.attributes uses an invalid type for an id: Attributes");
}