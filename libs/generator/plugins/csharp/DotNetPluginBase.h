#pragma once

#include "../TranslatorPlugin.h"

#include "generator/lang/CSharpHelper.h"

namespace holgen {
class DotNetPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
protected:
  CSharpMethod CreateMethod(const ClassMethod& method, InteropType interopType) const;
};
} // namespace holgen
