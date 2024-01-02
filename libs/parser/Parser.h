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

  struct DecoratorAttributeDefinition {
    std::string mName;
    std::string mValue;
  };

  // TODO: This is not a decorator, find a better name
  // TODO: Store supported decorator names and their attributes somewhere
  struct DecoratorDefinition {
    std::string mName;
    std::vector<DecoratorAttributeDefinition> mAttributes;
  };

  struct FieldDefinition {
    std::string mType;
    std::string mName;
    std::vector<DecoratorDefinition> mDecorators;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<DecoratorDefinition> mDecorators;
  };

  // rename to ParsedProject or ProjectDefinition to match the other classes?
  struct Project {
    std::vector<StructDefinition> mStructs;
  };

  class Parser {
    Project mProject;
    Tokenizer *mCurTokenizer = nullptr;
    void ParseStruct(StructDefinition &structDefinition);
    void ParseDecorator(DecoratorDefinition &decoratorDefinition);
    // FIXME: when using std::string, these will always construct/format the string. maybe use a macro instead?
    void
    GetAndExpectNext(Token &token, TokenType expectedType, const char *errorOnEOF, const char *errorOnTypeMismatch);
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);

    const Project &GetProject() const { return mProject; }
  };

}
