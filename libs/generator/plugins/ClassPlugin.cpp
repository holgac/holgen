#include "ClassPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void ClassPlugin::Run() {
  for (auto &structDefinition: mProject.mProject.mStructs) {
    if (structDefinition.mIsMixin)
      continue;
    AddClass(structDefinition);
  }
  for (auto &enumDefinition: mProject.mProject.mEnums) {
    auto cls = Class{enumDefinition.mName, mSettings.mNamespace, &enumDefinition};
    FillComments(enumDefinition, cls.mComments);
    Validate().NewClass(cls);
    mProject.mClasses.push_back(std::move(cls));
  }
}

void ClassPlugin::AddClass(const StructDefinition &structDefinition) {
  auto cls = Class{structDefinition.mName, mSettings.mNamespace, &structDefinition};
  FillComments(structDefinition, cls.mComments);
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}
} // namespace holgen
