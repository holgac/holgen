#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

class CWrappersPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessClass(Class& cls);
  void WrapMethod(Class& cls, const ClassMethod& method);

  Type ConvertType(const Type& type, bool isReturnType);
};

} // namespace holgen
