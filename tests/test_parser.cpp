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
    auto& s = proj.mStructs[0];
    EXPECT_EQ(s.mName, "a");
    EXPECT_EQ(s.mFields.size(), 2);
    EXPECT_EQ(s.mFields[0].mType, "s32");
    EXPECT_EQ(s.mFields[0].mName, "f1");
    EXPECT_EQ(s.mFields[1].mType, "float");
    EXPECT_EQ(s.mFields[1].mName, "f2");
  }

  TEST(ParserTest, StructDecorators) {
    Tokenizer tokenizer(R"DELIM(
  @dec1()
  @dec2(a1, a2=5, a3, a4=someStr)
  struct a   {
    s32 f1;
    float f2;
  }
)DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto &proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    auto& s = proj.mStructs[0];
    EXPECT_EQ(s.mDecorators.size(), 2);
    auto& ds = s.mDecorators;
    EXPECT_EQ(ds[0].mName, "dec1");
    EXPECT_EQ(ds[0].mAttributes.size(), 0);
    EXPECT_EQ(ds[1].mName, "dec2");
    EXPECT_EQ(ds[1].mAttributes.size(), 4);
    auto& dsa = ds[1].mAttributes;
    EXPECT_EQ(dsa[0].mName, "a1");
    EXPECT_EQ(dsa[0].mValue, "");
    EXPECT_EQ(dsa[1].mName, "a2");
    EXPECT_EQ(dsa[1].mValue, "5");
    EXPECT_EQ(dsa[2].mName, "a3");
    EXPECT_EQ(dsa[2].mValue, "");
    EXPECT_EQ(dsa[3].mName, "a4");
    EXPECT_EQ(dsa[3].mValue, "someStr");
  }

}
