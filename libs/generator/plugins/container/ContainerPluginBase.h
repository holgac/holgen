#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ContainerPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  [[nodiscard]] virtual bool ShouldProcess(const ClassField& field) const;
};
} // namespace holgen
