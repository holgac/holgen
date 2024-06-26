#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ClassPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
  void AddClass(const StructDefinition &structDefinition);
};
} // namespace holgen
