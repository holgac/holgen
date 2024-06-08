#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ClassRuleOfFivePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessClass(Class &cls);
};
} // namespace holgen
