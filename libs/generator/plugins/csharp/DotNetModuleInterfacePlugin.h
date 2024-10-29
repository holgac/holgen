#pragma once

#include "../TranslatorPlugin.h"
#include "DotNetPluginBase.h"

namespace holgen {
class DotNetModuleInterfacePlugin : public DotNetPluginBase {
public:
  using DotNetPluginBase::DotNetPluginBase;
  void Run() override;

private:
  void Process(Class &cls) const;
};
} // namespace holgen
