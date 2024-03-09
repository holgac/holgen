#include "ClassPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassPlugin::Run() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      auto &cls = mProject.mClasses.emplace_back(structDefinition.mName);
      cls.mStruct = &structDefinition;
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      auto &cls = mProject.mClasses.emplace_back(enumDefinition.mName);
      cls.mEnum = &enumDefinition;
    }
  }
}
