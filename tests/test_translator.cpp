#include <gtest/gtest.h>
#include "generator/Translator.h"

using namespace holgen;

namespace {

  void ExpectTypeEqual(const Type &expected, const Type &actual) {
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mType, expected.mType);
    EXPECT_EQ(actual.mIsConst, expected.mIsConst);
  }

  void ExpectClassFieldEqual(const ClassField &expected, const ClassField &actual) {
    EXPECT_EQ(actual.mVisibility, expected.mVisibility);
    EXPECT_EQ(actual.mName, expected.mName);
    ExpectTypeEqual(expected.mType, actual.mType);
  }

  void ExpectClassMethodArgumentEqual(const ClassMethodArgument &expected, const ClassMethodArgument &actual) {
    EXPECT_EQ(actual.mName, expected.mName);
    ExpectTypeEqual(expected.mType, actual.mType);
  }

  void ExpectClassMethodEqual(const ClassMethod &expected, const ClassMethod &actual) {
    EXPECT_EQ(actual.mVisibility, expected.mVisibility);
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mIsConst, expected.mIsConst);
    ExpectTypeEqual(expected.mType, actual.mType);
    for (size_t i = 0; i < expected.mArguments.size(); ++i) {
      ExpectClassMethodArgumentEqual(expected.mArguments[i], actual.mArguments[i]);
    }
    EXPECT_EQ(expected.mArguments.size(), actual.mArguments.size());
  }

  void ExpectFields(const Class &c, const std::vector<ClassField> &fields) {
    EXPECT_EQ(c.mFields.size(), fields.size());
    // This makes assumptions about the order
    for (size_t i = 0; i < c.mFields.size(); ++i) {
      ExpectClassFieldEqual(c.mFields[i], fields[i]);
    }
    EXPECT_EQ(c.mFields.size(), fields.size());
  }

  TEST(TranslatorTest, ClassFields) {
    Tokenizer tokenizer(R"DELIM(
  struct Person   {
    s32 age;
    float gender;
  }
    )DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto tp = Translator().Translate(parser.GetProject());
    EXPECT_EQ(tp.mClasses.size(), 1);
    auto &c = tp.mClasses[0];
    EXPECT_EQ(c.mName, "Person");
    ExpectFields(c, {
        ClassField{Visibility::Private, Type{"int32_t", false, TypeType::Value}, "mAge"},
        ClassField{Visibility::Private, Type{"float", false, TypeType::Value}, "mGender"},
    });
  }
}
