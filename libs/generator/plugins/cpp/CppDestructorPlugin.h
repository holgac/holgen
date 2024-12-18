#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class CppDestructorPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStructDefinition(Class &cls, ClassDestructor &destructor,
                               const StructDefinition &structDefinition);
  void ProcessVariantFields(ClassDestructor &destructor,
                            const StructDefinition &structDefinition) const;
};
} // namespace holgen
