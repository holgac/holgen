#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"

using namespace holgen;

namespace {
  void TestTokenizerResult(const char *text, std::vector<Token> expectedTokens) {
    Tokenizer tokenizer(text);
    for (const auto &expected: expectedTokens) {
      Token actual;
      EXPECT_TRUE(tokenizer.GetNext(actual));
      EXPECT_EQ(expected.mType, actual.mType);
      EXPECT_EQ(expected.mContents, actual.mContents);
    }
    Token tmp;
    EXPECT_FALSE(tokenizer.GetNext(tmp));
  }

  void TestTokenizerResultNonWhitespace(const char *text, std::vector<Token> expectedTokens) {
    Tokenizer tokenizer(text);
    for (const auto &expected: expectedTokens) {
      Token actual;
      EXPECT_TRUE(tokenizer.GetNextNonWhitespace(actual));
      EXPECT_EQ(expected.mType, actual.mType);
      EXPECT_EQ(expected.mContents, actual.mContents);
    }
    Token tmp;
    EXPECT_FALSE(tokenizer.GetNextNonWhitespace(tmp));
  }

  TEST(TokenizerTest, EmptyString) {
    TestTokenizerResult("", {});
  }

  TEST(TokenizerTest, Equation) {
    TestTokenizerResult("a   =b+c;", {
        Token{TokenType::String, "a"},
        Token{TokenType::Whitespace, "   "},
        Token{TokenType::Equals, "="},
        Token{TokenType::String, "b"},
        Token{TokenType::Plus, "+"},
        Token{TokenType::String, "c"},
        Token{TokenType::SemiColon, ";"},
    });
  }

  TEST(TokenizerTest, EquationNonWhitespace) {
    TestTokenizerResultNonWhitespace("a   =\nb       +\t\t\t \n\t     \tc;", {
        Token{TokenType::String, "a"},
        Token{TokenType::Equals, "="},
        Token{TokenType::String, "b"},
        Token{TokenType::Plus, "+"},
        Token{TokenType::String, "c"},
        Token{TokenType::SemiColon, ";"},
    });
  }

}
