#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class CompositeIdTypePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  [[nodiscard]] bool ShouldProcess(const Class& cls) const;
  void Process(Class& cls) const;
  void AddConstructor(Class& cls) const;
  void AddIsValidMethod(Class& cls) const;
  void AddLessThanOperator(Class& cls) const;
};
}
