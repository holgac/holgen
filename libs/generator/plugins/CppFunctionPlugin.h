#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class CppFunctionPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void AddCppFunction(Class &cls, const FunctionDefinition &functionDefinition);
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
};
}
