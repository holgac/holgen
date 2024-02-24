#pragma once

#include <map>
#include "Parser.h"

namespace holgen {

  class Validator {
  public:
    Validator(const ProjectDefinition &projectDefinition);
    void Validate();
  private:
    const ProjectDefinition &mProject;
    void Validate(const StructDefinition &structDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                  const TypeDefinition &typeDefinition);
    void Validate(const StructDefinition &structDefinition, const DecoratorDefinition &decoratorDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                  const DecoratorDefinition &decoratorDefinition);
    std::map<std::string, std::pair<TypeDefinition, TypeDefinition>> mJsonConverters;
  };

}
