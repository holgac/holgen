#include "TranslatedProject.h"

// TODO: dup code
#define GEN_GETTER_BY_NAME(cls_name, retval, getterName, field) \
const retval *cls_name::getterName(const std::string &name) const { \
for (const auto &entry: field) { \
if (entry.mName == name) \
return &entry; \
} \
return nullptr; \
}

namespace holgen {

  TranslatedProject::TranslatedProject(
      const ProjectDefinition &projectDefinition
  ) : mProject(projectDefinition), mDependencyGraph(mProject) {}

  Class *TranslatedProject::GetClass(const std::string &name) {
    for (auto &cls: mClasses) {
      if (cls.mName == name)
        return &cls;
    }
    return nullptr;
  }

  const Class *TranslatedProject::GetClass(const std::string &name) const {
    for (auto &cls: mClasses) {
      if (cls.mName == name)
        return &cls;
    }
    return nullptr;
  }

  ClassField *Class::GetField(const std::string &name) {
    for (auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }

  ClassMethod *Class::GetMethod(const std::string &name, bool isConst) {
    Constness constness = isConst ? Constness::Const : Constness::NotConst;
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

}

