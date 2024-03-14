#include "Parser.h"
#include <set>
#include <cstdint>
#include "core/Exception.h"
#include "core/St.h"

#define NEXT_OR_THROW(curToken, ...) \
  THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), __VA_ARGS__);

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
      } else if (curToken.mContents == "enum") {
        auto &enumDefinition = mProject.mEnums.emplace_back();
        enumDefinition.mAnnotations = std::move(annotations);
        ParseEnum(enumDefinition);
      } else {
        THROW("Unexpected token: {}", curToken.mContents);
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
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
    while (curToken.mType != TokenType::CClose) {
      while (curToken.mType == TokenType::At) {
        ParseAnnotation(curToken, annotations.emplace_back());
      }
      if (curToken.mType == TokenType::String) {
        if (curToken.mContents == "func") {
          FunctionDefinition &fd = structDefinition.mFunctions.emplace_back();
          fd.mAnnotations = std::move(annotations);
          annotations.clear();
          ParseFunction(curToken, fd);
        } else {
          auto &fieldDefinition = structDefinition.mFields.emplace_back();
          fieldDefinition.mAnnotations = std::move(annotations);
          annotations.clear();
          ParseField(curToken, fieldDefinition);
        }
      } else {
        THROW("Unexpected token in parsing struct: \"{}\"!", curToken.mContents)
      }
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
    }
  }

  void Parser::ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition) {
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
    THROW_IF(curToken.mType != TokenType::String, "Annotation name should be a string, found \"{}\"",
             curToken.mContents)
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
    THROW_IF(curToken.mType != TokenType::String, "Field name should be a string, found \"{}\"!", curToken.mContents);
    fieldDefinition.mName = curToken.mContents;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
    if (curToken.mType == TokenType::Equals) {
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
      THROW_IF(curToken.mType != TokenType::String, "Default value should be a string, found \"{}\"",
               curToken.mContents);
      fieldDefinition.mDefaultValue = curToken.mContents;
      THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
    }
    THROW_IF(curToken.mType != TokenType::SemiColon, "Field definition should be terminated with a ';', found \"{}\"",
             curToken.mContents);
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

  void Parser::ParseEnum(EnumDefinition &enumDefinition) {
    Token curToken;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition");
    THROW_IF(curToken.mType != TokenType::String, "Enum name should be a string, found \"{}\"", curToken.mContents);
    enumDefinition.mName = curToken.mContents;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition: {}",
             enumDefinition.mName);
    THROW_IF(curToken.mType != TokenType::COpen, "Enum name should be followed by a '{{', found \"{}\"",
             curToken.mContents);
    std::vector<AnnotationDefinition> annotations;
    THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition: {}",
             enumDefinition.mName);
    while (curToken.mType != TokenType::CClose) {
      while (curToken.mType == TokenType::At) {
        ParseAnnotation(curToken, annotations.emplace_back());
      }
      THROW_IF(curToken.mType != TokenType::String, "Unexpected token in parsing enum: \"{}\"", curToken.mContents);
      auto &enumEntryDefinition = enumDefinition.mEntries.emplace_back();
      enumEntryDefinition.mAnnotations = std::move(annotations);
      annotations.clear();
      ParseEnumEntry(curToken, enumDefinition, enumEntryDefinition);
      NEXT_OR_THROW(curToken, "Incomplete enum definition: {}", enumDefinition.mName);
    }
    int64_t nextValue = 0;
    std::set<int64_t> takenValues;
    for (auto &entry: enumDefinition.mEntries) {
      if (!entry.mValue.empty()) {
        takenValues.insert(strtoll(entry.mValue.data(), nullptr, 10));
      }
    }
    for (auto &entry: enumDefinition.mEntries) {
      if (!entry.mValue.empty())
        continue;
      while (takenValues.contains(nextValue)) ++nextValue;
      entry.mValue = std::format("{}", nextValue);
      ++nextValue;
    }

    while (takenValues.contains(nextValue)) ++nextValue;
    enumDefinition.mInvalidValue = std::format("{}", nextValue);
  }

  void Parser::ParseEnumEntry(
      Token &curToken, EnumDefinition &enumDefinition, EnumEntryDefinition &enumEntryDefinition) {
    enumEntryDefinition.mName = curToken.mContents;
    NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
    if (curToken.mType == TokenType::SemiColon) {
      return;
    }
    THROW_IF(curToken.mType != TokenType::Equals, "Enum entry name should be followed by a ';' or a '='");
    NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
    bool isNegative = false;
    if (curToken.mType == TokenType::Minus) {
      isNegative = true;
      NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
    }
    THROW_IF(curToken.mType != TokenType::String,
             "Enum values should be strings, found \"{}\"", curToken.mContents);
    THROW_IF(
        !St::IsIntegral(curToken.mContents),
        "Enum values should be positive integral strings, found \"{}\"", curToken.mContents);
    if (isNegative)
      enumEntryDefinition.mValue = std::string("-") + std::string(curToken.mContents);
    else
      enumEntryDefinition.mValue = curToken.mContents;
    NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
    THROW_IF(curToken.mType != TokenType::SemiColon,
             "Enum entry definition should be terminated by a ';', found \"{}\"", curToken.mContents);
  }

  void Parser::ParseFunction(Token &curToken, FunctionDefinition &fd) {
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    THROW_IF(curToken.mType != TokenType::String,
             "Function name should be a string, found \"{}\"", curToken.mContents);
    fd.mName = curToken.mContents;
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    if (curToken.mType == TokenType::SemiColon)
      return;
    THROW_IF(curToken.mType != TokenType::POpen,
             "Function call should start with a '(', found \"{}\"", curToken.mContents);
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    while (curToken.mType != TokenType::PClose) {
      FunctionArgumentDefinition &arg = fd.mArguments.emplace_back();
      ParseType(curToken, arg.mType);
      THROW_IF(curToken.mType != TokenType::String,
               "Function argument names should be strings, found \"{}\"", curToken.mContents);
      arg.mName = curToken.mContents;
      NEXT_OR_THROW(curToken, "Incomplete function definition!");
      if (curToken.mType == TokenType::Comma) {
        NEXT_OR_THROW(curToken, "Incomplete function definition!");
      }
    }
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    if (curToken.mType == TokenType::SemiColon) {
      fd.mReturnType.mName = "void";
      return;
    }
    THROW_IF(curToken.mType != TokenType::Minus,
             "Missing '-' in function syntax, found \"{}\"", curToken.mContents);
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    THROW_IF(curToken.mType != TokenType::AClose,
             "Missing '>' in function syntax, found \"{}\"", curToken.mContents);
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    ParseType(curToken, fd.mReturnType);
    THROW_IF(curToken.mType != TokenType::SemiColon,
             "Function definition should be terminated by a ';', found \"{}\"", curToken.mContents);
  }
}
