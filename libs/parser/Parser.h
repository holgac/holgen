#pragma once

#include <string>
#include <vector>
#include "tokenizer/Tokenizer.h"
#include "ProjectDefinition.h"

namespace holgen {
  class Parser {
    ProjectDefinition mProject;
    Tokenizer *mCurTokenizer = nullptr;
    void ParseStruct(StructDefinition &structDefinition);
    void ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition);
    void ParseField(Token &curToken, FieldDefinition &fieldDefinition);
    void ParseType(Token &curToken, TypeDefinition &typeDefinition);
  public:
    Parser();
    void Parse(Tokenizer &tokenizer);

    const ProjectDefinition &GetProject() const { return mProject; }
  };

}
