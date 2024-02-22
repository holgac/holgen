#pragma once

#include <string>
#include <vector>
#include "tokenizer/Tokenizer.h"

/*
 * specific for json + lua + binary?
 * handle versioning?
 */
namespace holgen {

  class ParserException : std::exception {
    std::string mMsg;
  public:
    explicit ParserException(std::string msg) : mMsg(std::move(msg)) {}

    const char *what() { return mMsg.c_str(); }
  };

  struct TypeDefinition {
    std::string mName;
    std::vector<TypeDefinition> mTemplateParameters;
  };

  // @decorator(attribute=5)
  struct DecoratorAttributeDefinition {
    std::string mName;
    // TODO: make it a TypeDefinition to support templating
    TypeDefinition mValue;
  };

  // TODO: This is not a decorator, find a better name
  // TODO: Store supported decorator names and their attributes somewhere
  struct DecoratorDefinition {
    std::string mName;
    std::vector<DecoratorAttributeDefinition> mAttributes;
    const DecoratorAttributeDefinition* GetAttribute(const std::string& name) const;
  };

  struct FieldDefinition {
    TypeDefinition mType;
    std::string mName;
    std::vector<DecoratorDefinition> mDecorators;
    const DecoratorDefinition* GetDecorator(const std::string& name) const;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<DecoratorDefinition> mDecorators;
    const DecoratorDefinition* GetDecorator(const std::string& name) const;
    const FieldDefinition* GetField(const std::string& name) const;
  };

  // rename to ParsedProject or ProjectDefinition to match the other classes?
  struct ProjectDefinition {
    std::vector<StructDefinition> mStructs;
  };

  class Parser {
    ProjectDefinition mProject;
    Tokenizer *mCurTokenizer = nullptr;
    void ParseStruct(StructDefinition &structDefinition);
    void ParseDecorator(DecoratorDefinition &decoratorDefinition);
    void ParseField(Token &curToken, FieldDefinition &fieldDefinition);
    void ParseType(Token &curToken, TypeDefinition &typeDefinition);
    // FIXME: when using std::string, these will always construct/format the string. maybe use a macro instead?
    void
    GetAndExpectNext(Token &token, TokenType expectedType, const char *errorOnEOF, const char *errorOnTypeMismatch);
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);

    const ProjectDefinition &GetProject() const { return mProject; }
  };

}
