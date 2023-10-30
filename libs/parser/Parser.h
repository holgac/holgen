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
    ParserException(const char *msg) : mMsg(msg) {}

    const char *what() { return mMsg; }
  };

  class DecoratorDefinition {
    std::string mName;
    std::string mValue;
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
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);
    void ParseStruct(StructDefinition &structDefinition);

    const Project& GetProject() const { return mProject; }
  };

}
