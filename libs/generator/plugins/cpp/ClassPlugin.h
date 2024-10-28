#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ClassPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void AddClass(const StructDefinition &structDefinition);
  void AddEnum(const EnumDefinition &enumDefinition);
};
} // namespace holgen
