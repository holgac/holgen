#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class DotNetModulePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void Process(Class& cls);
};
} // namespace holgen
