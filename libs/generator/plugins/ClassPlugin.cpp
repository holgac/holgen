#include "ClassPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void ClassPlugin::Run() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      if (structDefinition.mIsMixin)
        continue;
      auto cls = Class{structDefinition.mName, mSettings.mNamespace, &structDefinition};
      // TODO: operator==
      FillComments(structDefinition, cls.mComments);
      Validate().NewClass(cls);
      mProject.mClasses.push_back(std::move(cls));
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      auto cls = Class{enumDefinition.mName, mSettings.mNamespace, &enumDefinition};
      FillComments(enumDefinition, cls.mComments);
      Validate().NewClass(cls);
      mProject.mClasses.push_back(std::move(cls));
    }
  }
}
