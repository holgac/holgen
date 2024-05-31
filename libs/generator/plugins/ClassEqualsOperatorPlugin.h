#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ClassEqualsOperatorPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessClass(Class &cls);
  std::string GetEpsilon(const Class &cls, const ClassField &field);
};
} // namespace holgen
