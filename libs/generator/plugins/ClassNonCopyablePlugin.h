#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ClassNonCopyablePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
};
}