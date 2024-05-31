#include "Parser.h"
#include <cstdint>
#include <set>
#include "core/Exception.h"
#include "core/St.h"

#define PARSER_THROW_IF(cond, str, ...)                                                                                \
  THROW_IF(cond, "In {}:{}:{}: " str, mCurTokenizer->GetSource(), mCurTokenizer->GetLine() + 1,                        \
           mCurTokenizer->GetColumn() + 1, ##__VA_ARGS__);

#define NEXT_OR_THROW(curToken, ...) PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), __VA_ARGS__);

namespace holgen {
Parser::Parser() {}

void Parser::Parse(Tokenizer &tokenizer) {
  PARSER_THROW_IF(mCurTokenizer != nullptr, "Parse should not be called recursively!")
  mCurTokenizer = &tokenizer;
  while (true) {
    std::vector<AnnotationDefinition> annotations;
    Token curToken;
    if (!tokenizer.GetNextNonWhitespace(curToken))
      break;
    while (curToken.mType == TokenType::At) {
      ParseAnnotation(curToken, annotations.emplace_back());
    }
    PARSER_THROW_IF(curToken.mType != TokenType::String, "Expected a string, received \"{}\"!", curToken.mContents)
    if (curToken.mContents == "struct" || curToken.mContents == "mixin") {
      auto &structDefinition = mProject.mStructs.emplace_back();
      structDefinition.mAnnotations = std::move(annotations);
      structDefinition.mIsMixin = curToken.mContents == "mixin";
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
  FillDefinitionSource(structDefinition.mDefinitionSource);
  Token curToken;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Struct name should be a string, found \"{}\"",
                  curToken.mContents)
  structDefinition.mName = curToken.mContents;

  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
  if (curToken.mType == TokenType::Colon) {
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
    PARSER_THROW_IF(curToken.mType != TokenType::String, "Struct mixin should be a string, found \"{}\"",
                    curToken.mContents)
    while (true) {
      structDefinition.mMixins.emplace_back(curToken.mContents);
      PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
      if (curToken.mType == TokenType::Comma) {
        PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
        PARSER_THROW_IF(curToken.mType != TokenType::String, "Struct mixin should be a string, found \"{}\"",
                        curToken.mContents)
      } else {
        break;
      }
    }
  }
  PARSER_THROW_IF(curToken.mType != TokenType::COpen, "Struct name should be followed by a '{{', found \"{}\"",
                  curToken.mContents)

  std::vector<AnnotationDefinition> annotations;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
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
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete struct definition!")
  }
}

void Parser::ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition) {
  FillDefinitionSource(annotationDefinition.mDefinitionSource);
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Annotation name should be a string, found \"{}\"",
                  curToken.mContents)
  annotationDefinition.mName = curToken.mContents;

  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
  if (curToken.mType != TokenType::POpen)
    return;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
  while (curToken.mType != TokenType::PClose) {
    PARSER_THROW_IF(curToken.mType != TokenType::String, "Annotation attribute name should be a string, found \"{}\"!",
                    curToken.mContents);
    ParseAnnotationAttribute(curToken, annotationDefinition.mAttributes.emplace_back());
    if (curToken.mType == TokenType::Comma) {
      PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
    }
  }
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation definition!")
}

void Parser::ParseField(Token &curToken, FieldDefinition &fieldDefinition) {
  FillDefinitionSource(fieldDefinition.mDefinitionSource);
  ParseType(curToken, fieldDefinition.mType);
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Field name should be a string, found \"{}\"!",
                  curToken.mContents);
  fieldDefinition.mName = curToken.mContents;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
  if (curToken.mType == TokenType::Equals) {
    // TODO: test this
    PARSER_THROW_IF(!fieldDefinition.mType.mArraySize.empty(), "Arrays cannot have default values");
    NEXT_OR_THROW(curToken, "Incomplete field definition!");
    fieldDefinition.mDefaultValue = ParseDefaultValue(curToken);
  }
  PARSER_THROW_IF(curToken.mType != TokenType::SemiColon,
                  "Field definition should be terminated with a ';', found \"{}\"", curToken.mContents);
}

namespace {
template <typename T>
struct NameComparator {
  bool operator()(const T &lhs, const T &rhs) {
    return lhs.mName < rhs.mName;
  }
};
} // namespace

void Parser::PostProcess() {
  std::sort(mProject.mStructs.begin(), mProject.mStructs.end(), NameComparator<StructDefinition>{});
  std::sort(mProject.mMixins.begin(), mProject.mMixins.end(), NameComparator<StructDefinition>{});
  std::sort(mProject.mEnums.begin(), mProject.mEnums.end(), NameComparator<EnumDefinition>{});
}

std::string Parser::ParseDefaultValue(Token &curToken) {
  std::string defaultValue;
  if (curToken.mType == TokenType::Minus) {
    defaultValue += curToken.mContents;
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
  }
  if (curToken.mType == TokenType::String) {
    defaultValue += curToken.mContents;
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
  }
  if (curToken.mType == TokenType::Period) {
    defaultValue += curToken.mContents;
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
    if (curToken.mType == TokenType::String) {
      defaultValue += curToken.mContents;
      PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!");
    }
  }
  return defaultValue;
}

void Parser::ParseType(Token &curToken, TypeDefinition &typeDefinition) {
  typeDefinition.mName = curToken.mContents;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete type definition!")
  if (curToken.mType == TokenType::AOpen) {
    do {
      PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete type definition!")
      PARSER_THROW_IF(curToken.mType != TokenType::String, "Type definition should start with a string, found \"{}\"",
                      curToken.mContents)
      ParseType(curToken, typeDefinition.mTemplateParameters.emplace_back());
    } while (curToken.mType == TokenType::Comma);
    PARSER_THROW_IF(curToken.mType != TokenType::AClose,
                    "Templated type definition should be terminated with a '>', found \"{}\"", curToken.mContents)

    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
  }
  if (curToken.mType == TokenType::BOpen) {
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
    PARSER_THROW_IF(curToken.mType != TokenType::String,
                    "Invalid array size specifier, expected an integer or enum name, found \"{}\"", curToken.mContents)

    typeDefinition.mArraySize = curToken.mContents;
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
    PARSER_THROW_IF(curToken.mType != TokenType::BClose,
                    "Array type definition should be terminated with a ']', found \"{}\"", curToken.mContents)
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete field definition!")
  }
}

void Parser::ParseEnum(EnumDefinition &enumDefinition) {
  FillDefinitionSource(enumDefinition.mDefinitionSource);
  Token curToken;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition");
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Enum name should be a string, found \"{}\"",
                  curToken.mContents);
  enumDefinition.mName = curToken.mContents;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition: {}",
                  enumDefinition.mName);
  PARSER_THROW_IF(curToken.mType != TokenType::COpen, "Enum name should be followed by a '{{', found \"{}\"",
                  curToken.mContents);
  std::vector<AnnotationDefinition> annotations;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete enum definition: {}",
                  enumDefinition.mName);
  while (curToken.mType != TokenType::CClose) {
    while (curToken.mType == TokenType::At) {
      ParseAnnotation(curToken, annotations.emplace_back());
    }
    PARSER_THROW_IF(curToken.mType != TokenType::String, "Unexpected token in parsing enum: \"{}\"",
                    curToken.mContents);
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
    while (takenValues.contains(nextValue))
      ++nextValue;
    entry.mValue = std::format("{}", nextValue);
    ++nextValue;
  }

  while (takenValues.contains(nextValue))
    ++nextValue;
  enumDefinition.mInvalidValue = std::format("{}", nextValue);
}

void Parser::ParseEnumEntry(Token &curToken, EnumDefinition &enumDefinition, EnumEntryDefinition &enumEntryDefinition) {
  FillDefinitionSource(enumEntryDefinition.mDefinitionSource);
  enumEntryDefinition.mName = curToken.mContents;
  NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
  if (curToken.mType == TokenType::SemiColon) {
    return;
  }
  PARSER_THROW_IF(curToken.mType != TokenType::Equals, "Enum entry name should be followed by a ';' or a '='");
  NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
  bool isNegative = false;
  if (curToken.mType == TokenType::Minus) {
    isNegative = true;
    NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
  }
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Enum values should be strings, found \"{}\"",
                  curToken.mContents);
  PARSER_THROW_IF(!St::IsIntegral(curToken.mContents), "Enum values should be positive integral strings, found \"{}\"",
                  curToken.mContents);
  if (isNegative)
    enumEntryDefinition.mValue = std::string("-") + std::string(curToken.mContents);
  else
    enumEntryDefinition.mValue = curToken.mContents;
  NEXT_OR_THROW(curToken, "Incomplete enum entry definition: {}.{}", enumDefinition.mName, enumEntryDefinition.mName);
  PARSER_THROW_IF(curToken.mType != TokenType::SemiColon,
                  "Enum entry definition should be terminated by a ';', found \"{}\"", curToken.mContents);
}

void Parser::ParseFunction(Token &curToken, FunctionDefinition &fd) {
  FillDefinitionSource(fd.mDefinitionSource);
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Function name should be a string, found \"{}\"",
                  curToken.mContents);
  fd.mName = curToken.mContents;
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  if (curToken.mType == TokenType::SemiColon)
    return;
  PARSER_THROW_IF(curToken.mType != TokenType::POpen, "Function call should start with a '(', found \"{}\"",
                  curToken.mContents);
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  while (curToken.mType != TokenType::PClose) {
    ParseFunctionArgument(curToken, fd.mArguments.emplace_back());
  }
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  if (curToken.mType == TokenType::SemiColon) {
    fd.mReturnType.mName = "void";
    return;
  }
  PARSER_THROW_IF(curToken.mType != TokenType::Minus, "Missing '-' in function syntax, found \"{}\"",
                  curToken.mContents);
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  PARSER_THROW_IF(curToken.mType != TokenType::AClose, "Missing '>' in function syntax, found \"{}\"",
                  curToken.mContents);
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  ParseType(curToken, fd.mReturnType);
  PARSER_THROW_IF(curToken.mType != TokenType::SemiColon,
                  "Function definition should be terminated by a ';', found \"{}\"", curToken.mContents);
}

void Parser::ParseAnnotationAttribute(Token &curToken, AnnotationAttributeDefinition &annotationAttributeDefinition) {
  FillDefinitionSource(annotationAttributeDefinition.mDefinitionSource);
  annotationAttributeDefinition.mName = curToken.mContents;
  PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation attribute definition!")
  if (curToken.mType == TokenType::Equals) {
    PARSER_THROW_IF(!mCurTokenizer->GetNextNonWhitespace(curToken), "Incomplete annotation attribute definition!")
    PARSER_THROW_IF(curToken.mType != TokenType::String, "Annotation attribute should be a string, found \"{}\"",
                    curToken.mContents)
    ParseType(curToken, annotationAttributeDefinition.mValue);
  }
}

void Parser::FillDefinitionSource(DefinitionSource &definitionSource) {
  definitionSource.mSource = mCurTokenizer->GetSource();
  definitionSource.mLine = mCurTokenizer->GetLine();
  definitionSource.mColumn = mCurTokenizer->GetColumn();
}

void Parser::ParseFunctionArgument(Token &curToken, FunctionArgumentDefinition &arg) {
  FillDefinitionSource(arg.mDefinitionSource);
  ParseType(curToken, arg.mType);
  PARSER_THROW_IF(curToken.mType != TokenType::String, "Function argument names should be strings, found \"{}\"",
                  curToken.mContents);
  arg.mName = curToken.mContents;
  NEXT_OR_THROW(curToken, "Incomplete function definition!");
  if (curToken.mType == TokenType::String) {
    if (curToken.mContents == "out") {
      arg.mIsOut = true;
    } else if (curToken.mContents != "in") {
      THROW("Unknown argument qualifier \"{}\"", curToken.mContents);
    }
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
  }
  if (curToken.mType == TokenType::Equals) {
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
    arg.mDefaultValue = ParseDefaultValue(curToken);
  }
  if (curToken.mType == TokenType::Comma) {
    NEXT_OR_THROW(curToken, "Incomplete function definition!");
  }
}

} // namespace holgen
