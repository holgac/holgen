#pragma once
#include "../TranslatorPlugin.h"

namespace holgen {

class DotNetClassPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void Process(Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const;

};
} // namespace holgen
