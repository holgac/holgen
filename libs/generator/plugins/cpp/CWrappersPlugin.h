#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class CWrappersPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  bool ShouldProcess(const Class& cls) const;
  void ProcessClass(Class& cls) const;
  void WrapMethod(Class& cls, const ClassMethod& method) const;
};

} // namespace holgen
