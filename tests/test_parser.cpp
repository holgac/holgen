#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"

using namespace holgen;

namespace {
  TEST(ParserTest, EmptyStruct) {
    Tokenizer tokenizer("struct a   { }");
    Parser parser;
    parser.Parse(tokenizer);
    auto& proj = parser.GetProject();
    EXPECT_EQ(proj.mStructs.size(), 1);
    EXPECT_EQ(proj.mStructs[0].mName, "a");
  }

}
