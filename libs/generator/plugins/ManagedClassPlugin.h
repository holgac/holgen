#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ManagedClassPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void GenerateStaticGetter(Class &cls, const Class &manager, const ClassField &managerField) const;
  void GenerateIndexGetter(Class &cls, const Class &manager, const ClassField &managerField,
                           const AnnotationDefinition &annotationDefinition) const;
};
}
