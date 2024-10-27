#include "DotNetModulePlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void DotNetModulePlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    Process(cls);
  }
}

void DotNetModulePlugin::Process(Class &cls) {
  cls.mClassType = ClassType::Struct;
  cls.mFields.emplace_back(Naming().FieldNameInCpp("name"), Type{"std::string"},
                           Visibility::Public);
}
} // namespace holgen
