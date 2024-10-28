#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ClassIdFieldPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
};
} // namespace holgen
