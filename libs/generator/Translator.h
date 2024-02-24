#pragma once

#include <vector>
#include <string>
#include "core/LineWithAction.h"
// Needed for Project (TODO: move to separate file and fwd declare)
#include "parser/Parser.h"
#include "TranslatedProject.h"


namespace holgen {
  class Translator {
    void GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const;
    void ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const;
    void ProcessContainerField(Class &generatedClass, const FieldDefinition &fieldDefinition) const;
    const ProjectDefinition* mProject = nullptr;
  public:
    TranslatedProject Translate(const ProjectDefinition &project);
  };
}
