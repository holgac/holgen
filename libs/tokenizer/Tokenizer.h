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
    POpen, // (
    PClose, // )
    COpen, // {
    CClose, // }
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
    // bool operator==(const Token& rhs) const { return mType == rhs.mType && mContents == rhs.mContents;}
  };

  class Tokenizer {
  private:
    std::string_view mData;
    size_t mIndex;
    size_t mEndIndex;
  public:
    Tokenizer(std::string_view sv);
    bool GetCurrent(Token& tok);
    bool GetNext(Token& tok);
    bool GetNextNonWhitespace(Token& tok);
    // void Next();
  };
}
