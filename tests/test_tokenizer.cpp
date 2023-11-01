#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"

using namespace holgen;
// TODO: some failure caseS

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

  TEST(TokenizerTest, Comments) {
    TestTokenizerResult("a   =//b+c;\nb / /*aaaaa*/ c;", {
        Token{TokenType::String, "a"},
        Token{TokenType::Whitespace, "   "},
        Token{TokenType::Equals, "="},
        Token{TokenType::Comment, "//b+c;"},
        Token{TokenType::Whitespace, "\n"},
        Token{TokenType::String, "b"},
        Token{TokenType::Whitespace, " "},
        Token{TokenType::Slash, "/"},
        Token{TokenType::Whitespace, " "},
        Token{TokenType::Comment, "/*aaaaa*/"},
        Token{TokenType::Whitespace, " "},
        Token{TokenType::String, "c"},
        Token{TokenType::SemiColon, ";"},
    });
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
    TestTokenizerResultNonWhitespace("a /*comment*/  =\nb       +\t\t\t \n\t     \tc;/", {
        Token{TokenType::String, "a"},
        Token{TokenType::Equals, "="},
        Token{TokenType::String, "b"},
        Token{TokenType::Plus, "+"},
        Token{TokenType::String, "c"},
        Token{TokenType::SemiColon, ";"},
        Token{TokenType::Slash, "/"},
    });
  }

  TEST(TokenizerTest, Quotes) {
    TestTokenizerResultNonWhitespace("a /*comment*/  = 'hello how are you';", {
      Token{TokenType::String, "a"},
      Token{TokenType::Equals, "="},
      Token{TokenType::String, "hello how are you"},
      Token{TokenType::SemiColon, ";"},
      });
  }

}
