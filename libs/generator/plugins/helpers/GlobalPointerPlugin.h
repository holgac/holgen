#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
// This is like a singleton, but the user is expected to handle object deletion
class GlobalPointerPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
  void GenerateSetInstance(Class &cls) const;
  void GenerateGetInstance(Class &cls) const;
  void GenerateInstanceField(Class &cls) const;
};
} // namespace holgen
