#pragma once

#include <string>
#include <vector>
#include "ProjectDefinition.h"
#include "tokenizer/Tokenizer.h"

namespace holgen {
class Parser {
  ProjectDefinition &mProject;
  Tokenizer &mTokenizer;
  void ParseFunction(Token &curToken, FunctionDefinition &functionDefinition);
  void ParseFunctionArgument(Token &curToken, FunctionArgumentDefinition &arg);
  void ParseStruct(StructDefinition &structDefinition);
  void ParseEnum(EnumDefinition &enumDefinition);
  void ParseEnumEntry(Token &curToken, EnumDefinition &enumDefinition,
                      EnumEntryDefinition &enumEntryDefinition);
  void ParseAnnotation(Token &curToken, AnnotationDefinition &annotationDefinition);
  void ParseAnnotationAttribute(Token &curToken,
                                AnnotationAttributeDefinition &annotationAttributeDefinition);
  void ParseField(Token &curToken, FieldDefinition &fieldDefinition);
  void ParseType(Token &curToken, TypeDefinition &typeDefinition);
  void FillDefinitionSource(DefinitionSource &definitionSource);

public:
  Parser(ProjectDefinition &project, Tokenizer &tokenizer);
  void Parse();

  // TODO: this shouldn't be here
  static void PostProcess(ProjectDefinition &project);

  std::string ParseDefaultValue(Token &curToken);
};

} // namespace holgen
