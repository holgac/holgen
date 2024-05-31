#pragma once

#include <string>
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
  BOpen, // [
  BClose, // ]
  Slash, // /
  At, // @
};

struct Token {
  TokenType mType;
  std::string_view mContents;
  // bool operator==(const Token& rhs) const { return mReturnType == rhs.mReturnType && mContents == rhs.mContents;}
};

class Tokenizer {
public:
  Tokenizer(std::string_view sv, std::string source);
  bool GetCurrent(Token &tok);
  bool GetNext(Token &tok);
  bool GetNextNonWhitespace(Token &tok);
  size_t GetLine() const;
  size_t GetColumn() const;
  std::string GetSource() const;

private:
  bool GetNextInner(Token &tok);
  std::string_view mData;
  size_t mIndex = 0;
  size_t mEndIndex = 0;
  size_t mLine = 0;
  size_t mColumn = 0;
  std::string mSource;
};
} // namespace holgen
