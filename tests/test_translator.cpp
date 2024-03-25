#include <gtest/gtest.h>
#include "generator/Translator.h"
#include "Helpers.h"

using namespace holgen;

namespace {

  void ExpectClassFieldEqual(const ClassField &expected, const ClassField &actual) {
    EXPECT_EQ(actual.mVisibility, expected.mVisibility);
    EXPECT_EQ(actual.mName, expected.mName);
    helpers::ExpectTypeEqual(expected.mType, actual.mType);
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
    auto tp = Translator(parser.GetProject()).Translate();
    auto &c = tp.mClasses[0];
    EXPECT_EQ(c.mName, "Person");
    ExpectFields(c, {
        ClassField{"mAge",
                   Type("int32_t", PassByType::Value, Constness::NotConst),
                   Visibility::Private, Staticness::NotStatic, ""},
        ClassField{"mGender",
                   Type("float", PassByType::Value, Constness::NotConst),
                   Visibility::Private, Staticness::NotStatic, ""},
    });
  }

  TEST(TranslatorTest, Containers) {
    Tokenizer tokenizer(R"DELIM(
  struct City {
    @id
    u32 id;
    string name;
  }
  struct Country {
    @container(elemName=city)
    deque<City> cities;
  }
    )DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto tp = Translator(parser.GetProject()).Translate();
    auto country = tp.GetClass("Country");
    ASSERT_NE(country, nullptr);
    EXPECT_NE(country->GetMethod("GetCities", Constness::NotConst), nullptr);
    EXPECT_NE(country->GetMethod("GetCities", Constness::Const), nullptr);
    auto method = country->GetMethod("GetCities", Constness::NotConst);
    ASSERT_NE(method, nullptr);
    EXPECT_EQ(method->mArguments.size(), 0);
    method = country->GetMethod("GetCity", Constness::Const);
    ASSERT_NE(method, nullptr);
    EXPECT_EQ(method->mArguments.size(), 1);
    EXPECT_EQ(method->mArguments[0].mType.mName, "uint32_t");
  }

  TEST(TranslatorTest, ContainerIndex) {
    Tokenizer tokenizer(R"DELIM(
  struct City {
    @id
    u32 id;
    string name;
  }
  struct Country {
    @container(elemName=city)
    @index(on=name)
    deque<City> cities;
  }
    )DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto tp = Translator(parser.GetProject()).Translate();
    auto country = tp.GetClass("Country");
    ASSERT_NE(country, nullptr);
    auto method = country->GetMethod("GetCityFromName", Constness::Const);
    ASSERT_NE(method, nullptr);
    EXPECT_EQ(method->mArguments.size(), 1);
    EXPECT_EQ(method->mArguments[0].mType.mName, "std::string");
  }
}
