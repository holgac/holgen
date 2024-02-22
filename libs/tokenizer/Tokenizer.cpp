#include "Tokenizer.h"
#include <map>
#include "core/Exception.h"

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
        {'<', TokenType::AOpen},
        {'>', TokenType::AClose},
        {'+', TokenType::Plus},
        {'/', TokenType::Slash},
        {'@', TokenType::At},
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
    if (c == '/') {
      if (mIndex + 1 < mData.size()) {
        if (mData[mIndex + 1] == '/') {
          // read line comment
          mEndIndex = mIndex + 2;
          while (mEndIndex < mData.size() && !(mData[mEndIndex] == '\n' && mData[mEndIndex - 1] != '\\')) {
            ++mEndIndex;
          }
          tok.mType = TokenType::Comment;
          tok.mContents = mData.substr(mIndex, mEndIndex - mIndex);
          return true;
        } else if (mData[mIndex + 1] == '*') {
          // read block comment
          mEndIndex = mIndex + 3;
          while (mEndIndex < mData.size() && !(mData[mEndIndex] == '/' && mData[mEndIndex - 1] == '*')) {
            ++mEndIndex;
          }
          ++mEndIndex;
          tok.mType = TokenType::Comment;
          tok.mContents = mData.substr(mIndex, mEndIndex - mIndex);
          return true;
        }
      }
    }
    if (IsWhitespace(c)) {
      ++mEndIndex;
      while (mEndIndex < mData.size() && IsWhitespace(mData[mEndIndex])) {
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
    if (c == '"' || c == '\'') {
      ++mEndIndex;
      while (mEndIndex < mData.size() && mData[mEndIndex] != c)
        ++mEndIndex;
      THROW_IF(mEndIndex == mData.size(), "Malformed string: {}", mData.substr(mIndex + 1))
      ++mEndIndex;
      tok.mType = TokenType::String;
      // Comment tokens' contents include the comment special chars, but string literals don't, for ease of use
      // Is that ok?
      // If this becomes an issue, can have mContents and mActualContents
      tok.mContents = mData.substr(mIndex + 1, mEndIndex - mIndex - 2);
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
      if (tok.mType != TokenType::Whitespace && tok.mType != TokenType::Comment)
        return true;
    }
  }
}
