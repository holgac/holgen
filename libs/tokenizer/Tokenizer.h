#pragma once

#include <string_view>

namespace holgen {
  enum class TokenType {
    Whitespace,
    String,
    Comment,
    Period,
    Comma,
    Colon,
    SemiColon,
    Equals,
    Plus,
    Minus,
    POpen, // (
    PClose, // )
    COpen, // {
    CClose, // }
    AOpen, // <
    AClose, // >
    Slash, // /
    At, // @
    /*
    BOpen, // [
    BClose, // ]
     */
  };

  struct Token {
    TokenType mType;
    std::string_view mContents;
    // bool operator==(const Token& rhs) const { return mReturnType == rhs.mReturnType && mContents == rhs.mContents;}
  };

  class Tokenizer {
  private:
    std::string_view mData;
    size_t mIndex;
    size_t mEndIndex;
  public:
    // TODO(RELEASE): string source arg for easier debugging
    // TODO(RELEASE): calculate line and column
    Tokenizer(std::string_view sv);
    bool GetCurrent(Token& tok);
    bool GetNext(Token& tok);
    bool GetNextNonWhitespace(Token& tok);
    // void Next();
  };
}
