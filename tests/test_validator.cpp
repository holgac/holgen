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

TEST_F(ValidatorTest, InvalidNames) {
  ExpectErrorMessage(R"DELIM(
struct template {
  s8 field;
}
  )DELIM", "Struct template uses a reserved keyword.");
  ExpectErrorMessage(R"DELIM(
struct struct {
  s8 field;
}
  )DELIM", "Struct struct uses a reserved keyword.");
  ExpectErrorMessage(R"DELIM(
struct A {
  s8 int8_t;
}
  )DELIM", "Field A.int8_t uses a reserved keyword.");
}

TEST_F(ValidatorTest, DuplicateNames) {
  ExpectErrorMessage(R"DELIM(
struct A {
  s8 field;
}
struct A {
  s16 field;
}
  )DELIM", "Duplicate struct name: A");
  ExpectErrorMessage(R"DELIM(
struct A {
  s8 field;
  s8 field;
}
  )DELIM", "Duplicate field name: A.field");
}

TEST_F(ValidatorTest, UnknownTypes) {
  ExpectErrorMessage(R"DELIM(
struct A {
  someType field;
}
  )DELIM", "Field A.field uses an unknown type: someType");

  ExpectErrorMessage(R"DELIM(
struct A {
  s8 field;
}
struct B {
  A a;
  C c;
}
  )DELIM", "Field B.c uses an unknown type: C");

  ExpectErrorMessage(R"DELIM(
struct A {
  s8 field;
}
struct B {
  map<string, map<string, map<string, A>>> aMap;
  map<string, map<string, map<string, C>>> cMap;
}
  )DELIM", "Field B.cMap uses an unknown type: C");
}

TEST_F(ValidatorTest, JsonConvert) {
  ExpectErrorMessage(R"DELIM(
struct A {
  @jsonConvert(using=aaa)
  u32 field;
}
  )DELIM", "Field A.field has annotation jsonConvert with missing attribute: from");
  ExpectErrorMessage(R"DELIM(
struct A {
  @jsonConvert(from=string)
  u32 field;
}
  )DELIM", "Field A.field has annotation jsonConvert with missing attribute: using");
  ExpectErrorMessage(R"DELIM(
struct A {
  @jsonConvert(from=string,using=myFunc)
  @jsonConvert(from=u8,using=myFunc2)
  u32 field;
}
  )DELIM", "Field A.field has multiple jsonConvert annotations");
  ExpectErrorMessage(R"DELIM(
struct A {
  @jsonConvert(from=string,using=myFunc)
  u32 field;
}
struct B {
  @jsonConvert(from=string,using=myFunc)
  u8 field;
}
  )DELIM", "Converter myFunc cannot convert to multiple types: u32 and u8");
  ExpectErrorMessage(R"DELIM(
struct A {
  @jsonConvert(from=u8,using=myFunc)
  u32 field;
}
struct B {
  @jsonConvert(from=string,using=myFunc)
  u32 field;
}
  )DELIM", "Converter myFunc cannot convert from multiple types: u8 and string");
}

TEST_F(ValidatorTest, Container) {
  ExpectErrorMessage(R"DELIM(
struct Person {
  u32 field;
}
struct B {
  @container()
  vector<Person> people;
}
  )DELIM", "Field B.people has annotation container with missing attribute: elemName");
}

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

TEST_F(ValidatorTest, DataManager) {
  ExpectErrorMessage(R"DELIM(
@managed(by=DataMan, field=people)
struct Person {
  @id()
  u32 id;
}
  )DELIM", "Struct Person references a DataManager DataMan that does not exist");

  ExpectErrorMessage(R"DELIM(
@managed(by=DataMan, field=people)
struct Person {
  @id()
  u32 id;
}
struct DataMan {
}
  )DELIM", "Struct Person references a DataManager DataMan that does not have the dataManager annotation!");

  ExpectErrorMessage(R"DELIM(
@managed(by=DataMan, field=people)
struct Person {
  @id()
  u32 id;
}

@dataManager
struct DataMan {
}
  )DELIM", "Struct Person references DataManager field DataMan.people that does not exist");

  ExpectErrorMessage(R"DELIM(
@managed(by=DataMan, field=people)
struct Person {
  @id()
  u32 id;
}

@dataManager
struct DataMan {
  vector<Person> people;
}
  )DELIM", "Struct Person references DataManager field DataMan.people that is not a container");
  return;

}
TEST_F(ValidatorTest, Ref) {
  ExpectErrorMessage(R"DELIM(
struct Person {
  @id
  u64 id;
  Ref partner;
}
  )DELIM", "Ref field Person.partner should have a single template parameter");
  ExpectErrorMessage(R"DELIM(
struct Person {
  @id
  u64 id;
  Ref<Person, Person> partner;
}
  )DELIM", "Ref field Person.partner should have a single template parameter");
  ExpectErrorMessage(R"DELIM(
struct Person {
  @id
  u64 id;
  Ref<Person> partner;
}
  )DELIM", "Ref field Person.partner references Person which is not managed");
}
