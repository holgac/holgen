#pragma once

#include <string>
#include <vector>
#include "tokenizer/Tokenizer.h"
#include "ProjectDefinition.h"

namespace holgen {
  class Parser {
    // TODO: don't store ProjectDefinition here. Instead store a reference to it and pass via ctor
    ProjectDefinition mProject;
    Tokenizer *mCurTokenizer = nullptr;
    void ParseFunction(Token &curToken, FunctionDefinition &functionDefinition);
    void ParseStruct(StructDefinition &structDefinition);
    void ParseEnum(EnumDefinition &enumDefinition);
    void ParseEnumEntry(Token &curToken, EnumDefinition &enumDefinition, EnumEntryDefinition &enumEntryDefinition);
    void ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition);
    void ParseField(Token &curToken, FieldDefinition &fieldDefinition);
    void ParseType(Token &curToken, TypeDefinition &typeDefinition);
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);

    const ProjectDefinition &GetProject() const { return mProject; }
  };

}
