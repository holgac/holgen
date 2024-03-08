#pragma once

#include <vector>
#include <string>
#include "core/LineWithAction.h"
// Needed for Project (TODO: move to separate file and fwd declare)
#include "parser/Parser.h"
#include "TranslatedProject.h"
// TODO: register custom generators
// #include "generators/Generator.h"


namespace holgen {
  class Translator {
  public:
    TranslatedProject Translate(const ProjectDefinition &project);
  private:
    void GenerateEnum(Class &generatedClass, const EnumDefinition &enumDefinition) const;
    void GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const;
    void ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const;
    void ProcessManagedClass(Class &generatedClass) const;
    void ProcessRefField(Class &generatedClass, ClassField &generatedField,
                         const FieldDefinition &fieldDefinition) const;
    void ProcessContainerField(Class &generatedClass, const FieldDefinition &fieldDefinition) const;
    const ProjectDefinition *mProject = nullptr;
  };
}
