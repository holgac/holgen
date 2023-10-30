#include "Tokenizer.h"
#include <map>

namespace holgen {
  namespace {
    std::map<char, TokenType> SpecialTokens = {
        {'.', TokenType::Period},
        {',', TokenType::Comma},
        {':', TokenType::Colon},
        {';', TokenType::SemiColon},
        {'=', TokenType::Equals},
        {'(', TokenType::POpen},
        {')', TokenType::PClose},
        {'{', TokenType::COpen},
        {'}', TokenType::CClose},
        {'+', TokenType::Plus},
    };

    bool IsWhitespace(char c) {
      return c == ' ' || c == '\n' || c == '\t';
    }
  }

  Tokenizer::Tokenizer(std::string_view sv) : mData(sv), mEndIndex(0) {
  }

  bool Tokenizer::GetNext(Token &tok) {
    if (mEndIndex == mData.size())
      return false;
    mIndex = mEndIndex;
    char c = mData[mIndex];
    if (IsWhitespace(c)) {
      ++mEndIndex;
      while (IsWhitespace(mData[mEndIndex])) {
        ++mEndIndex;
      }
      tok.mType = TokenType::Whitespace;
      tok.mContents = mData.substr(mIndex, mEndIndex - mIndex);
      return true;
    }
    auto it = SpecialTokens.find(c);
    if (it != SpecialTokens.end()) {
      tok.mType = it->second;
      tok.mContents = mData.substr(mIndex, 1);
      mEndIndex = mIndex + 1;
      return true;
    }
    while (true) {
      ++mEndIndex;
      char c = mData[mEndIndex];
      if (IsWhitespace(c) || SpecialTokens.find(c) != SpecialTokens.end())
        break;
    }
    tok.mType = TokenType::String;
    tok.mContents = mData.substr(mIndex, mEndIndex - mIndex);
    return true;
  }

  bool Tokenizer::GetNextNonWhitespace(Token &tok) {
    while (true) {
      if (!GetNext(tok))
        return false;
      if (tok.mType != TokenType::Whitespace)
        return true;
    }
  }
}
