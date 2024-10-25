#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

class CWrappersPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessClass(Class& cls, bool singleton);
  void WrapMethod(Class& cls, const ClassMethod& method, bool singleton);

  Type ConvertType(const Type& type);
};

} // namespace holgen
