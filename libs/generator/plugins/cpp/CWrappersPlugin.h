#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class CWrappersPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  bool ShouldProcess(const Class &cls) const;
  void ProcessClass(Class &cls) const;
  void WrapMethod(Class &cls, const ClassMethod &method) const;
  Type SanitizeType(const Type &type) const;
  bool ShouldProcess(const ClassMethod &method) const;
  bool ShouldProcess(const Type &type) const;
};

} // namespace holgen
