#include "Parser.h"
// #include <format>
#include <cstdint>

namespace holgen {

  Parser::Parser() {

  }

  void Parser::Parse(Tokenizer &tokenizer) {
    if (mCurTokenizer != nullptr)
      throw ParserException("Parse should not be called recursively!");
    mCurTokenizer = &tokenizer;
    while (true) {
      Token curToken;
      if (!tokenizer.GetNextNonWhitespace(curToken))
        break;
      // TODO: parse decorators
      if (curToken.mType != TokenType::String) {
        // throw ParserException(std::format("Expected a string, received {0}!", curToken.mContents));
        throw ParserException("Expected a string!");
      }
      if (curToken.mContents == "struct") {
        ParseStruct(mProject.mStructs.emplace_back());
      }
    }
    mCurTokenizer = nullptr;
  }

  void Parser::ParseStruct(StructDefinition& structDefinition) {
    Token curToken;
    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete struct definition!");
    if (curToken.mType != TokenType::String)
      throw ParserException("Struct name should be a string!");
    structDefinition.mName = curToken.mContents;
    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete struct definition!");
    if (curToken.mType != TokenType::COpen)
      throw ParserException("Expected '{'!");
    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete struct definition!");
    // TODO: parse fields and their decorators
    if (curToken.mType != TokenType::CClose)
      throw ParserException("Expected '}'!");
  }
}