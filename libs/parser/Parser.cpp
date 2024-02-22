#include "Parser.h"
#include <format>

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
        throw ParserException(std::format("Expected a string, received \"{0}\"!", curToken.mContents));
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
    std::vector<DecoratorDefinition> decorators;
    while (true) {
      if (!mCurTokenizer->GetNextNonWhitespace(curToken))
        throw ParserException("Incomplete struct definition!");
      if (curToken.mType == TokenType::CClose)
        break;
      if (curToken.mType == TokenType::String) {
        // TODO: check if curToken.mContents == "func"
        auto& fieldDefinition = structDefinition.mFields.emplace_back();
        fieldDefinition.mDecorators = std::move(decorators);
        decorators.clear();
        ParseField(curToken, fieldDefinition);
      } else if (curToken.mType == TokenType::At) {
        ParseDecorator(decorators.emplace_back());
      } else {
        throw ParserException(std::format("Unexpected token in parsing struct: \"{}\"!", curToken.mContents));
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
        throw ParserException(
            std::format("Decorator attribute name should be a string, found \"{0}\"!", curToken.mContents));
      auto &decoratorAttributeDefinition = decoratorDefinition.mAttributes.emplace_back();
      decoratorAttributeDefinition.mName = curToken.mContents;
      if (!mCurTokenizer->GetNextNonWhitespace(curToken))
        throw ParserException("Incomplete decorator attribute definition!");
      if (curToken.mType == TokenType::Equals) {
        GetAndExpectNext(curToken, TokenType::String, "Incomplete decorator attribute definition!",
                         "Malformed decorator attribute definition!");
        ParseType(curToken, decoratorAttributeDefinition.mValue);
      } else {
        if (!mCurTokenizer->GetNextNonWhitespace(curToken))
          throw ParserException("Incomplete decorator definition!");
      }
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

  void Parser::ParseField(Token &curToken, FieldDefinition &fieldDefinition) {
    ParseType(curToken, fieldDefinition.mType);
    if (curToken.mType != TokenType::String)
      throw ParserException(std::format("Field name should be a string, found \"{0}\"!", curToken.mContents));
    fieldDefinition.mName = curToken.mContents;
    GetAndExpectNext(curToken, TokenType::SemiColon, "Incomplete field definition!",
                     "Field definition should be terminated with a ';'");
  }

  void Parser::ParseType(Token &curToken, TypeDefinition &typeDefinition) {
    typeDefinition.mName = curToken.mContents;
    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete type definition!");
    if (curToken.mType != TokenType::AOpen)
      return;
    do {
      GetAndExpectNext(curToken, TokenType::String, "Malformed type definition!",
                       "Type definitions should start with a string");
      ParseType(curToken, typeDefinition.mTemplateParameters.emplace_back());
    } while (curToken.mType == TokenType::Comma);
    if (curToken.mType != TokenType::AClose)
      throw ParserException("Templated type definition should be terminated with a '>'");

    if (!mCurTokenizer->GetNextNonWhitespace(curToken))
      throw ParserException("Incomplete field definition!");
  }

  const DecoratorDefinition *FieldDefinition::GetDecorator(const std::string& name) const {
    for(const auto& decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const DecoratorAttributeDefinition *DecoratorDefinition::GetAttribute(const std::string &name) const {
    for (const auto& attribute: mAttributes) {
      if (attribute.mName == name)
        return &attribute;
    }
    return nullptr;
  }

  const DecoratorDefinition *StructDefinition::GetDecorator(const std::string &name) const {
    for(const auto& decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetField(const std::string &name) const {
    for(const auto& field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }
}