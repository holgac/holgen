#include "Parser.h"
#include "core/Exception.h"
#include "core/St.h"

namespace holgen {

  Parser::Parser() {

  }

  void Parser::Parse(Tokenizer &tokenizer) {
    THROW_IF(mCurTokenizer != nullptr, "Parse should not be called recursively!")
    mCurTokenizer = &tokenizer;
    while (true) {
      std::vector<AnnotationDefinition> annotations;
      Token curToken;
      if (!tokenizer.GetNextNonWhitespace(curToken))
        break;
      while (curToken.mType == TokenType::At) {
        ParseAnnotation(curToken, annotations.emplace_back());
      }
      THROW_IF(curToken.mType != TokenType::String, "Expected a string, received \"{}\"!", curToken.mContents)
      if (curToken.mContents == "struct") {
        auto &structDefinition = mProject.mStructs.emplace_back();
        structDefinition.mAnnotations = std::move(annotations);
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

    std::vector<AnnotationDefinition> annotations;
    while (true) {
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
      if (curToken.mType == TokenType::CClose)
        break;
      while (curToken.mType == TokenType::At) {
        ParseAnnotation(curToken, annotations.emplace_back());
      }
      if (curToken.mType == TokenType::String) {
        // TODO: check if curToken.mContents == "func"
        auto &fieldDefinition = structDefinition.mFields.emplace_back();
        fieldDefinition.mAnnotations= std::move(annotations);
        annotations.clear();
        ParseField(curToken, fieldDefinition);
      } else {
        THROW("Unexpected token in parsing struct: \"{}\"!", curToken.mContents)
      }
    }
  }

  void Parser::ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition) {
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
    THROW_IF(curToken.mType != TokenType::String, "Annotation name should be a string, found \"{}\"", curToken.mContents)
    annotationDefinition.mName = curToken.mContents;

    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
    if (curToken.mType != TokenType::POpen)
      return;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
    while (curToken.mType != TokenType::PClose) {
      THROW_IF(curToken.mType != TokenType::String, "Annotation attribute name should be a string, found \"{}\"!",
               curToken.mContents);
      auto &annotationAttributeDefinition = annotationDefinition.mAttributes.emplace_back();
      annotationAttributeDefinition.mName = curToken.mContents;
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation attribute definition!")
      if (curToken.mType == TokenType::Equals) {
        THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation attribute definition!")
        THROW_IF(curToken.mType != TokenType::String, "Annotation attribute should be a string, found \"{}\"",
                 curToken.mContents)
        ParseType(curToken, annotationAttributeDefinition.mValue);
      }
      if (curToken.mType == TokenType::Comma) {
        THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
      }
    }
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
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

}