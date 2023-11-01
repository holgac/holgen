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
      std::vector<DecoratorDefinition> decorators;
      Token curToken;
      if (!tokenizer.GetNextNonWhitespace(curToken))
        break;
      while (curToken.mType == TokenType::At) {
        ParseDecorator(decorators.emplace_back());
        if (!tokenizer.GetNextNonWhitespace(curToken))
          throw ParserException("Decorator definition without a matching type!");
      }
      if (curToken.mType != TokenType::String) {
        // throw ParserException(std::format("Expected a string, received {0}!", curToken.mContents));
        throw ParserException("Expected a string!");
      }
      if (curToken.mContents == "struct") {
        auto &structDefinition = mProject.mStructs.emplace_back();
        structDefinition.mDecorators = std::move(decorators);
        ParseStruct(structDefinition);
      }
    }
    mCurTokenizer = nullptr;
  }

  void Parser::ParseStruct(StructDefinition &structDefinition) {
    Token curToken;
    GetAndExpectNext(curToken, TokenType::String, "Incomplete struct definition!", "Struct name should be a string!");
    structDefinition.mName = curToken.mContents;
    GetAndExpectNext(curToken, TokenType::COpen, "Incomplete struct definition!",
                     "Struct name should be followed by a '{'");
    while (true) {
      if (!mCurTokenizer->GetNextNonWhitespace(curToken))
        throw ParserException("Incomplete struct definition!");
      if (curToken.mType == TokenType::CClose)
        break;
      if (curToken.mType == TokenType::String) {
        auto &field = structDefinition.mFields.emplace_back();
        field.mType = curToken.mContents;
        if (!mCurTokenizer->GetNextNonWhitespace(curToken))
          throw ParserException("Incomplete field definition!");
        if (curToken.mType != TokenType::String)
          throw ParserException("Field name should be a string!");
        field.mName = curToken.mContents;
      }
    }
  }

  void Parser::ParseDecorator(DecoratorDefinition &decoratorDefinition) {
    Token curToken;
    GetAndExpectNext(curToken, TokenType::String, "Incomplete decorator definition!",
                     "Decorator name should be a string!");
    decoratorDefinition.mName = curToken.mContents;
    GetAndExpectNext(curToken, TokenType::POpen, "Incomplete decorator definition!", "Malformed decorator definition!");
    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete decorator definition!");
    while (curToken.mType != TokenType::PClose) {
      if (curToken.mType != TokenType::String)
        throw ParserException("Decorator attribute name should be a string!");
      auto &decoratorAttributeDefinition = decoratorDefinition.mAttributes.emplace_back();
      decoratorAttributeDefinition.mName = curToken.mContents;
      if (!mCurTokenizer->GetNextNonWhitespace(curToken))
        throw ParserException("Incomplete decorator attribute definition!");
      if (curToken.mType == TokenType::Equals) {
        GetAndExpectNext(curToken, TokenType::String, "Incomplete decorator attribute definition!",
                         "Malformed decorator attribute definition!");
        decoratorAttributeDefinition.mValue = curToken.mContents;
      }
      if (!mCurTokenizer->GetNextNonWhitespace(curToken))
        throw ParserException("Incomplete decorator definition!");
      if (curToken.mType == TokenType::Comma) {
        if (!mCurTokenizer->GetNextNonWhitespace(curToken))
          throw ParserException("Incomplete decorator definition!");
      }

    }
  }

  void Parser::GetAndExpectNext(Token &token, TokenType expectedType, const char *errorOnEOF,
                                const char *errorOnTypeMismatch) {
    if (!mCurTokenizer->GetNextNonWhitespace(token))
      throw ParserException(errorOnEOF);
    if (token.mType != expectedType)
      throw ParserException(errorOnTypeMismatch);
  }
}