#include "TranslatedProject.h"

namespace holgen {

  TranslatedProject::TranslatedProject(
      const ProjectDefinition &projectDefinition
  ) : mProject(projectDefinition), mDependencyGraph(mProject) {}

  ClassField *Class::GetField(const std::string &name) {
    for (auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }
}

