#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class DotNetModuleInterfacePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void Process(Class &cls) const;
};
} // namespace holgen
