#pragma once

#include "../TranslatorPlugin.h"

#include "generator/lang/CSharpHelper.h"

namespace holgen {
class DotNetPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;

protected:
  [[nodiscard]] CSharpMethod CreateMethod(const Class &cls, const ClassMethod &method,
                                          InteropType interopType, bool addThisArgument,
                                          bool ignoreAuxiliaries) const;
  [[nodiscard]] CSharpConstructor CreateConstructor(const Class &cls, const ClassMethod &method,
                                                    InteropType interopType) const;
  void PopulateArguments(std::list<CSharpMethodArgument> &out,
                         const std::list<MethodArgument> &arguments, InteropType interopType,
                         bool ignoreAuxiliaries) const;
};
} // namespace holgen
