#include "ClassPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassPlugin::EnrichClasses() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      auto &cls = mProject.mClasses.emplace_back();
      cls.mName = structDefinition.mName;
      cls.mStruct = &structDefinition;
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      auto &cls = mProject.mClasses.emplace_back();
      cls.mName = enumDefinition.mName;
      cls.mEnum = &enumDefinition;
    }
  }
}
