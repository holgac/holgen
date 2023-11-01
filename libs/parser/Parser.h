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
    const char *mMsg;
  public:
    ParserException(const char *msg) : mMsg(msg) {

    }

    const char *what() { return mMsg; }
  };

  struct DecoratorAttributeDefinition {
    std::string mName;
    std::string mValue;
  };

  // TODO: This is not a decorator, find a better name
  struct DecoratorDefinition {
    std::string mName;
    std::vector<DecoratorAttributeDefinition> mAttributes;
  };

  struct FieldDefinition {
    std::string mType;
    std::string mName;
    std::vector<DecoratorDefinition> mDecorators;
  };

  // check thrift for supported types?
  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<DecoratorDefinition> mDecorators;
  };

  struct Project {
    std::vector<StructDefinition> mStructs;
  };

  class Parser {
    Project mProject;
    Tokenizer *mCurTokenizer = nullptr;
    void ParseStruct(StructDefinition &structDefinition);
    void ParseDecorator(DecoratorDefinition &decoratorDefinition);
    // FIXME: when using std::string, these will always construct/format the string
    void GetAndExpectNext(Token& token, TokenType expectedType, const char* errorOnEOF, const char* errorOnTypeMismatch);
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);

    const Project &GetProject() const { return mProject; }
  };

}
