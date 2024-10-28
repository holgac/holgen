#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ClassFieldSetterPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  bool IsNonCopyable(const std::string &structName);
  void Run() override;
};
} // namespace holgen
