#include "ClassPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassPlugin::Run() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      auto cls = Class(structDefinition.mName, &structDefinition);
      Validate().NewClass(cls);
      mProject.mClasses.push_back(std::move(cls));
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      auto cls = Class(enumDefinition.mName, &enumDefinition);
      Validate().NewClass(cls);
      mProject.mClasses.push_back(std::move(cls));
    }
  }
}
