#include "Parser.h"
#include "core/Exception.h"

namespace holgen {

  Parser::Parser() {

  }

  void Parser::Parse(Tokenizer &tokenizer) {
    THROW_IF(mCurTokenizer != nullptr, "Parse should not be called recursively!")
    mCurTokenizer = &tokenizer;
    while (true) {
      std::vector<DecoratorDefinition> decorators;
      Token curToken;
      if (!tokenizer.GetNextNonWhitespace(curToken))
        break;
      while (curToken.mType == TokenType::At) {
        ParseDecorator(decorators.emplace_back());
        THROW_IF(!tokenizer.GetNextNonWhitespace(curToken), "Decorator definition without a matching type!")
      }
      THROW_IF(curToken.mType != TokenType::String, "Expected a string, received \"{}\"!", curToken.mContents)
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
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
    THROW_IF(curToken.mType != TokenType::String, "Struct name should be a string, found \"{}\"", curToken.mContents)
    structDefinition.mName = curToken.mContents;

    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
    THROW_IF(curToken.mType != TokenType::COpen, "Struct name should be followed by a '{{', found \"{}\"",
             curToken.mContents)

    std::vector<DecoratorDefinition> decorators;
    while (true) {
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
      if (curToken.mType == TokenType::CClose)
        break;
      if (curToken.mType == TokenType::String) {
        // TODO: check if curToken.mContents == "func"
        auto &fieldDefinition = structDefinition.mFields.emplace_back();
        fieldDefinition.mDecorators = std::move(decorators);
        decorators.clear();
        ParseField(curToken, fieldDefinition);
      } else if (curToken.mType == TokenType::At) {
        ParseDecorator(decorators.emplace_back());
      } else {
        THROW("Unexpected token in parsing struct: \"{}\"!", curToken.mContents)
      }
    }
  }

  void Parser::ParseDecorator(DecoratorDefinition &decoratorDefinition) {
    Token curToken;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator definition!")
    THROW_IF(curToken.mType != TokenType::String, "Decorator name should be a string, found \"{}\"", curToken.mContents)
    decoratorDefinition.mName = curToken.mContents;

    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator definition!")
    THROW_IF(curToken.mType != TokenType::POpen, "Decorator name should be followed by a '(', found \"{}\"",
             curToken.mContents)
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator definition!")
    while (curToken.mType != TokenType::PClose) {
      THROW_IF(curToken.mType != TokenType::String, "Decorator attribute name should be a string, found \"{}\"!",
               curToken.mContents);
      auto &decoratorAttributeDefinition = decoratorDefinition.mAttributes.emplace_back();
      decoratorAttributeDefinition.mName = curToken.mContents;
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator attribute definition!")
      if (curToken.mType == TokenType::Equals) {
        THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator attribute definition!")
        THROW_IF(curToken.mType != TokenType::String, "Decorator attribute should be a string, found \"{}\"",
                 curToken.mContents)
        ParseType(curToken, decoratorAttributeDefinition.mValue);
      } else {
        THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator definition!")
      }
      if (curToken.mType == TokenType::Comma) {
        THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete decorator definition!")
      }
    }
  }

  void Parser::ParseField(Token &curToken, FieldDefinition &fieldDefinition) {
    ParseType(curToken, fieldDefinition.mType);
    THROW_IF(curToken.mType != TokenType::String, "Field name should be a string, found \"{}\"!", curToken.mContents)
    fieldDefinition.mName = curToken.mContents;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
    THROW_IF(curToken.mType != TokenType::SemiColon, "Field definition should be terminated with a ';', found \"{}\"",
             curToken.mContents)
  }

  void Parser::ParseType(Token &curToken, TypeDefinition &typeDefinition) {
    typeDefinition.mName = curToken.mContents;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete type definition!")
    if (curToken.mType != TokenType::AOpen)
      return;
    do {
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete type definition!")
      THROW_IF(curToken.mType != TokenType::String, "Type definition should start with a string, found \"{}\"",
               curToken.mContents)
      ParseType(curToken, typeDefinition.mTemplateParameters.emplace_back());
    } while (curToken.mType == TokenType::Comma);
    THROW_IF(curToken.mType != TokenType::AClose,
             "Templated type definition should be terminated with a '>', found \"{}\"", curToken.mContents)

    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
  }

  const DecoratorDefinition *FieldDefinition::GetDecorator(const std::string &name) const {
    for (const auto &decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const DecoratorAttributeDefinition *DecoratorDefinition::GetAttribute(const std::string &name) const {
    for (const auto &attribute: mAttributes) {
      if (attribute.mName == name)
        return &attribute;
    }
    return nullptr;
  }

  const DecoratorDefinition *StructDefinition::GetDecorator(const std::string &name) const {
    for (const auto &decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetField(const std::string &name) const {
    for (const auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }
}