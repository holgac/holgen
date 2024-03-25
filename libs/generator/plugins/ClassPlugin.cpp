#include "ClassPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassPlugin::Run() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      mProject.mClasses.emplace_back(structDefinition.mName, &structDefinition);
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      mProject.mClasses.emplace_back(enumDefinition.mName, &enumDefinition);
    }
  }
}
