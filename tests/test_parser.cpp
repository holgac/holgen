#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"

using namespace holgen;

namespace {
  TEST(ParserTest, EmptyStruct) {
    Tokenizer tokenizer("struct a   { }");
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
    )DELIM");
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

  TEST(ParserTest, FieldDecorators) {
    Tokenizer tokenizer(R"DELIM(
  struct a   {
    @dec1()
    @dec2(a1, a2=5, a3, a4='long string')
    s32 f1;
  }
    )DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mDecorators.size(), 0);
    EXPECT_EQ(s.mFields.size(), 1);
    auto& f = s.mFields[0];
    auto &ds = f.mDecorators;
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

  TEST(ParserTest, StructDecorators) {
    Tokenizer tokenizer(R"DELIM(
  @dec1()
  @dec2(a1, a2=5, a3, a4='long string')
  struct a   {
    s32 f1;
    float f2;
  }
)DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto &s = proj.mStructs[0];
    EXPECT_EQ(s.mDecorators.size(), 2);
    auto &ds = s.mDecorators;
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
    )DELIM");
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
    )DELIM");
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

}
