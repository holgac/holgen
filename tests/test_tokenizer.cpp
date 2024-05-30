#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "Helpers.h"

using namespace holgen;
// TODO: some failure cases

class TokenizerTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

namespace {
void TestTokenizerResult(const char *text, std::vector<Token> expectedTokens) {
  Tokenizer tokenizer(text, "TokenizerTest");
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
  Tokenizer tokenizer(text, "TokenizerTest");
  for (const auto &expected: expectedTokens) {
    Token actual;
    EXPECT_TRUE(tokenizer.GetNextNonWhitespace(actual));
    EXPECT_EQ(expected.mType, actual.mType);
    EXPECT_EQ(expected.mContents, actual.mContents);
  }
  Token tmp;
  EXPECT_FALSE(tokenizer.GetNextNonWhitespace(tmp));
}

void ExpectNext(Tokenizer &tokenizer, size_t line, size_t col, const Token &token) {
  Token actual;
  EXPECT_TRUE(tokenizer.GetNext(actual)) << " expected contents: " << token.mContents;
  EXPECT_EQ(actual.mType, token.mType) << " expected contents: " << token.mContents;
  EXPECT_EQ(tokenizer.GetLine(), line) << " expected contents: " << token.mContents;
  EXPECT_EQ(tokenizer.GetColumn(), col) << " expected contents: " << token.mContents;
  EXPECT_EQ(actual.mContents, token.mContents);
}
}

TEST_F(TokenizerTest, EmptyString) {
  TestTokenizerResult("", {});
}

TEST_F(TokenizerTest, Comments) {
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

TEST_F(TokenizerTest, Equation) {
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

TEST_F(TokenizerTest, EquationNonWhitespace) {
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

TEST_F(TokenizerTest, Quotes) {
  TestTokenizerResultNonWhitespace("a /*comment*/  = 'hello how are you';", {
      Token{TokenType::String, "a"},
      Token{TokenType::Equals, "="},
      Token{TokenType::String, "hello how are you"},
      Token{TokenType::SemiColon, ";"},
  });
}

TEST_F(TokenizerTest, AngleBrackets) {
  TestTokenizerResultNonWhitespace("map<int, vector<string>> myMap;", {
      Token{TokenType::String, "map"},
      Token{TokenType::AOpen, "<"},
      Token{TokenType::String, "int"},
      Token{TokenType::Comma, ","},
      Token{TokenType::String, "vector"},
      Token{TokenType::AOpen, "<"},
      Token{TokenType::String, "string"},
      Token{TokenType::AClose, ">"},
      Token{TokenType::AClose, ">"},
      Token{TokenType::String, "myMap"},
      Token{TokenType::SemiColon, ";"},
  });
}

TEST_F(TokenizerTest, LineAndColumn) {
  auto rawContents = R"R(
int i = 512  /* comment */
some_statement
//extra comment
/*
line comment
*/
int
  )R";
  auto contents = helpers::Trim(rawContents);
  Tokenizer tokenizer(contents, "TokenizerTest");
  EXPECT_EQ(tokenizer.GetLine(), 0);
  EXPECT_EQ(tokenizer.GetColumn(), 0);

  ExpectNext(tokenizer, 0, 0, Token{TokenType::String, "int"});
  ExpectNext(tokenizer, 0, 3, Token{TokenType::Whitespace, " "});
  ExpectNext(tokenizer, 0, 4, Token{TokenType::String, "i"});
  ExpectNext(tokenizer, 0, 5, Token{TokenType::Whitespace, " "});
  ExpectNext(tokenizer, 0, 6, Token{TokenType::Equals, "="});
  ExpectNext(tokenizer, 0, 7, Token{TokenType::Whitespace, " "});
  ExpectNext(tokenizer, 0, 8, Token{TokenType::String, "512"});
  ExpectNext(tokenizer, 0, 11, Token{TokenType::Whitespace, "  "});
  ExpectNext(tokenizer, 0, 13, Token{TokenType::Comment, "/* comment */"});
  ExpectNext(tokenizer, 0, 26, Token{TokenType::Whitespace, "\n"});
  ExpectNext(tokenizer, 1, 0, Token{TokenType::String, "some_statement"});
  ExpectNext(tokenizer, 1, 14, Token{TokenType::Whitespace, "\n"});
  ExpectNext(tokenizer, 2, 0, Token{TokenType::Comment, "//extra comment"});
  ExpectNext(tokenizer, 2, 15, Token{TokenType::Whitespace, "\n"});
  ExpectNext(tokenizer, 3, 0, Token{TokenType::Comment, "/*\nline comment\n*/"});
  ExpectNext(tokenizer, 5, 2, Token{TokenType::Whitespace, "\n"});
  ExpectNext(tokenizer, 6, 0, Token{TokenType::String, "int"});
}

