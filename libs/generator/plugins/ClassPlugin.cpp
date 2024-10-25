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
    AddEnum(enumDefinition);
  }
}

void ClassPlugin::AddClass(const StructDefinition &structDefinition) {
  auto cls = Class{structDefinition.mName, mSettings.mNamespace, &structDefinition};
  FillComments(structDefinition, cls.mComments);
  if (structDefinition.GetAnnotation(Annotations::Singleton)) {
    auto &base = cls.mBaseClasses.emplace_back(Visibility::Public, Type{"Singleton"});
    base.mType.mTemplateParameters.emplace_back(cls.mName);
  }
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void ClassPlugin::AddEnum(const EnumDefinition &enumDefinition) {
  auto cls = Class{enumDefinition.mName, mSettings.mNamespace, &enumDefinition};
  FillComments(enumDefinition, cls.mComments);
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}
} // namespace holgen
