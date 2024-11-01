#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class DeferredDeleterPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  Class &GenerateClass();
  void GenerateField(Class &cls);
  void GeneratePerformMethod(Class &cls);
  void GeneratePerformManagedMethod(Class &cls);
  void GeneratePerformManagedArrayMethod(Class &cls);
  void GenerateConstructor(Class &cls);
};

} // namespace holgen
