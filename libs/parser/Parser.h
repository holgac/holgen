#pragma once

#include <string>
#include <vector>
#include "tokenizer/Tokenizer.h"
#include "ProjectDefinition.h"

namespace holgen {
class Parser {
  // TODO: don't store ProjectDefinition here. Instead store a reference to it and pass via ctor
  ProjectDefinition mProject;
  // TODO: don't store Tokenizer ptr here. Instead store a reference to it and pass via ctor
  Tokenizer *mCurTokenizer = nullptr;
  void ParseFunction(Token &curToken, FunctionDefinition &functionDefinition);
  void ParseFunctionArgument(Token &curToken, FunctionArgumentDefinition &arg);
  void ParseStruct(StructDefinition &structDefinition);
  void ParseEnum(EnumDefinition &enumDefinition);
  void ParseEnumEntry(Token &curToken, EnumDefinition &enumDefinition, EnumEntryDefinition &enumEntryDefinition);
  void ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition);
  void ParseAnnotationAttribute(Token &curToken, AnnotationAttributeDefinition &annotationAttributeDefinition);
  void ParseField(Token &curToken, FieldDefinition &fieldDefinition);
  void ParseType(Token &curToken, TypeDefinition &typeDefinition);
  void FillDefinitionSource(DefinitionSource &definitionSource);
public:
  Parser();
  void Parse(Tokenizer &tokenizer);

  const ProjectDefinition &GetProject() const { return mProject; }

  std::string ParseDefaultValue(Token &curToken);
};

}
