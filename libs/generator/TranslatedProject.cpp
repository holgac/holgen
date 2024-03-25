#include "TranslatedProject.h"
#include "holgen.h"

namespace holgen {

  TranslatedProject::TranslatedProject(
      const ProjectDefinition &projectDefinition
  ) : mProject(projectDefinition), mDependencyGraph(mProject) {}

  ClassMethod *Class::GetMethod(const std::string &name, Constness constness) {
    for (auto &method: mMethods) {
      if (method.mName == name && method.mConstness == constness)
        return &method;
    }
    return nullptr;
  }

  const Typedef *Class::GetTypedef(const std::string &name) const {
    for (auto &typdef: mTypedefs) {
      if (typdef.mTargetType == name)
        return &typdef;
    }
    return nullptr;
  }

  GEN_GETTER_BY_NAME(TranslatedProject, Class, GetClass, mClasses);

  GEN_GETTER_BY_NAME_NONCONST(TranslatedProject, Class, GetClass, mClasses);

  GEN_GETTER_BY_NAME_NONCONST(Class, ClassField, GetField, mFields);
}

