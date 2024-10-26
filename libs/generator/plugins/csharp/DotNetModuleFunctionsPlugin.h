#pragma once

#include "../FunctionPluginBase.h"

namespace holgen {
class DotNetModuleFunctionsPlugin : public FunctionPluginBase {
public:
  using FunctionPluginBase::FunctionPluginBase;
  void Run() override;

private:
  void Process(Class& cls);
};
} // namespace holgen
