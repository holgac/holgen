#pragma once
#include "TranslatorPlugin.h"
#include "generator/TranslatedProject.h"

namespace holgen {
class FunctionPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  ClassMethod AddFunction(const FunctionDefinition &functionDefinition);
};
} // namespace holgen
