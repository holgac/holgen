#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"

using namespace holgen;

namespace {
  TEST(ParserTest, EmptyStruct) {
    Tokenizer tokenizer("struct a   { }", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    EXPECT_EQ(proj.mStructs[0].mName, "a");
  }

  TEST(ParserTest, StructFields) {
    Tokenizer tokenizer(R"DELIM(
  struct a   {
    s32 f1;
    float f2;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mName, "a");
    EXPECT_EQ(s.mFields.size(), 2);
    EXPECT_EQ(s.mFields[0].mType.mName, "s32");
    EXPECT_EQ(s.mFields[0].mName, "f1");
    EXPECT_EQ(s.mFields[1].mType.mName, "float");
    EXPECT_EQ(s.mFields[1].mName, "f2");
  }

  TEST(ParserTest, FieldAnnotations) {
    Tokenizer tokenizer(R"DELIM(
  struct a   {
    @dec1()
    @dec2(a1, a2=5, a3, a4='long string')
    s32 f1;
    @dec3(a)
    u32 f2;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mAnnotations.size(), 0);
    EXPECT_EQ(s.mFields.size(), 2);
    auto &f = s.mFields[0];
    auto &ds = f.mAnnotations;
    EXPECT_EQ(ds[0].mName, "dec1");
    EXPECT_EQ(ds[0].mAttributes.size(), 0);
    EXPECT_EQ(ds[1].mName, "dec2");
    EXPECT_EQ(ds[1].mAttributes.size(), 4);
    auto &dsa = ds[1].mAttributes;
    EXPECT_EQ(dsa[0].mName, "a1");
    EXPECT_EQ(dsa[0].mValue.mName, "");
    EXPECT_EQ(dsa[1].mName, "a2");
    EXPECT_EQ(dsa[1].mValue.mName, "5");
    EXPECT_EQ(dsa[2].mName, "a3");
    EXPECT_EQ(dsa[2].mValue.mName, "");
    EXPECT_EQ(dsa[3].mName, "a4");
    EXPECT_EQ(dsa[3].mValue.mName, "long string");
    auto &f2 = s.mFields[1];
    auto &ds2 = f2.mAnnotations;
    EXPECT_EQ(ds2.size(), 1);
    EXPECT_EQ(ds2[0].mName, "dec3");
    EXPECT_EQ(ds2[0].mAttributes.size(), 1);
    EXPECT_EQ(ds2[0].mAttributes[0].mName, "a");
    EXPECT_EQ(ds2[0].mAttributes[0].mValue.mName, "");
  }

  TEST(ParserTest, EmptyAnnotation) {
    Tokenizer tokenizer(R"DELIM(
  @dec1
  struct a   {
    @dec2
    s32 f1;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    ASSERT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mAnnotations.size(), 1);
    auto &ds = s.mAnnotations;
    EXPECT_EQ(ds[0].mName, "dec1");
    EXPECT_EQ(ds[0].mAttributes.size(), 0);
    auto f1 = s.GetField("f1");
    ASSERT_NE(f1, nullptr);
    auto &df = f1->mAnnotations;
    EXPECT_EQ(df[0].mName, "dec2");
    EXPECT_EQ(df[0].mAttributes.size(), 0);
  }

  TEST(ParserTest, StructAnnotations) {
    Tokenizer tokenizer(R"DELIM(
  @dec1()
  @dec2(a1, a2=5, a3, a4='long string')
  struct a   {
    s32 f1;
    float f2;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mAnnotations.size(), 2);
    auto &ds = s.mAnnotations;
    EXPECT_EQ(ds[0].mName, "dec1");
    EXPECT_EQ(ds[0].mAttributes.size(), 0);
    EXPECT_EQ(ds[1].mName, "dec2");
    EXPECT_EQ(ds[1].mAttributes.size(), 4);
    auto &dsa = ds[1].mAttributes;
    EXPECT_EQ(dsa[0].mName, "a1");
    EXPECT_EQ(dsa[0].mValue.mName, "");
    EXPECT_EQ(dsa[1].mName, "a2");
    EXPECT_EQ(dsa[1].mValue.mName, "5");
    EXPECT_EQ(dsa[2].mName, "a3");
    EXPECT_EQ(dsa[2].mValue.mName, "");
    EXPECT_EQ(dsa[3].mName, "a4");
    EXPECT_EQ(dsa[3].mValue.mName, "long string");
  }

  TEST(ParserTest, Templates) {
    Tokenizer tokenizer(R"DELIM(
  struct a   {
    map<u32, map<string, vector<float>>> myMap;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mName, "a");
    EXPECT_EQ(s.mFields.size(), 1);
    EXPECT_EQ(s.mFields[0].mName, "myMap");
    auto &t = s.mFields[0].mType;
    EXPECT_EQ(t.mName, "map");
    EXPECT_EQ(t.mTemplateParameters.size(), 2);
    EXPECT_EQ(t.mTemplateParameters[0].mName, "u32");
    EXPECT_EQ(t.mTemplateParameters[0].mTemplateParameters.size(), 0);
    EXPECT_EQ(t.mTemplateParameters[1].mName, "map");
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters.size(), 2);
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[0].mName, "string");
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[0].mTemplateParameters.size(), 0);
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mName, "vector");
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mTemplateParameters.size(), 1);
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mTemplateParameters[0].mName, "float");
    EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mTemplateParameters[0].mTemplateParameters.size(), 0);
  }

  TEST(ParserTest, NestedStructs) {
    Tokenizer tokenizer(R"DELIM(
  struct Sound {
    string name;
    u32 volume;
  }
  struct Animal {
    Sound sound;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 2);
    {

      auto &s = proj.mStructs[0];
      EXPECT_EQ(s.mName, "Sound");
      EXPECT_EQ(s.mFields.size(), 2);
      EXPECT_EQ(s.mFields[0].mType.mName, "string");
      EXPECT_EQ(s.mFields[0].mName, "name");
      EXPECT_EQ(s.mFields[1].mType.mName, "u32");
      EXPECT_EQ(s.mFields[1].mName, "volume");
    }

    {
      auto &s = proj.mStructs[1];
      EXPECT_EQ(s.mName, "Animal");
      EXPECT_EQ(s.mFields.size(), 1);
      EXPECT_EQ(s.mFields[0].mType.mName, "Sound");
      EXPECT_EQ(s.mFields[0].mName, "sound");
    }

  }

  TEST(ParserTest, Enums) {
    Tokenizer tokenizer(R"DELIM(
  enum LandscapeType {
    Land;
    River = 2;
    Sea;
    Mountain;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mEnums.size(), 1);
    auto e = proj.GetEnum("LandscapeType");
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(e->mEntries.size(), 4);
    EXPECT_EQ(e->mInvalidValue, "4");
    ASSERT_NE(e->GetEnumEntry("Land"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Land")->mValue, "0");
    ASSERT_NE(e->GetEnumEntry("Sea"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Sea")->mValue, "1");
    ASSERT_NE(e->GetEnumEntry("River"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("River")->mValue, "2");
    ASSERT_NE(e->GetEnumEntry("Mountain"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Mountain")->mValue, "3");
  }

  TEST(ParserTest, EnumInvalidEntry) {
    Tokenizer tokenizer(R"DELIM(
  enum LandscapeType {
    Land;
    River = -1;
    Sea;
    Mountain = -2;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mEnums.size(), 1);
    auto e = proj.GetEnum("LandscapeType");
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(e->mEntries.size(), 4);
    EXPECT_EQ(e->mInvalidValue, "2");
    ASSERT_NE(e->GetEnumEntry("Land"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Land")->mValue, "0");
    ASSERT_NE(e->GetEnumEntry("Sea"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Sea")->mValue, "1");
    ASSERT_NE(e->GetEnumEntry("River"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("River")->mValue, "-1");
    ASSERT_NE(e->GetEnumEntry("Mountain"), nullptr);
    EXPECT_EQ(e->GetEnumEntry("Mountain")->mValue, "-2");
  }

  TEST(ParserTest, Functions) {
    Tokenizer tokenizer(R"DELIM(
  struct Actor {
    string name;
  }
  struct Action {
    func func1(Actor actor);
    func func2(s32 i1, vector<s32> i2) -> void;
    func func3() -> vector<s32>;
  }
    )DELIM", "ParserTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    auto action = proj.GetStruct("Action");
    ASSERT_NE(action, nullptr);
    EXPECT_EQ(action->mFunctions.size(), 3);
    auto func = action->GetFunction("func1");
    ASSERT_NE(func, nullptr);
    ASSERT_EQ(func->mReturnType.mName, "void");
    ASSERT_EQ(func->mArguments.size(), 1);
    ASSERT_EQ(func->mArguments[0].mType.mName, "Actor");
    ASSERT_EQ(func->mArguments[0].mName, "actor");

    func = action->GetFunction("func2");
    ASSERT_NE(func, nullptr);
    ASSERT_EQ(func->mReturnType.mName, "void");
    ASSERT_EQ(func->mArguments.size(), 2);
    ASSERT_EQ(func->mArguments[0].mType.mName, "s32");
    ASSERT_EQ(func->mArguments[0].mName, "i1");
    ASSERT_EQ(func->mArguments[1].mType.mName, "vector");
    ASSERT_EQ(func->mArguments[1].mType.mTemplateParameters[0].mName, "s32");
    ASSERT_EQ(func->mArguments[1].mName, "i2");

    func = action->GetFunction("func3");
    ASSERT_NE(func, nullptr);
    ASSERT_EQ(func->mArguments.size(), 0);
    ASSERT_EQ(func->mReturnType.mName, "vector");
    ASSERT_EQ(func->mReturnType.mTemplateParameters[0].mName, "s32");
  }

}
