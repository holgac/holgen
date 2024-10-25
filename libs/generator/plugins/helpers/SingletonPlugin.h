#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class SingletonPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  Class &GenerateClass();
  void GenerateField(Class &cls);
  void GenerateGetter(Class &cls, bool pointer);
  void GenerateSetter(Class &cls);
  void GenerateDeleter(Class &cls);
};

} // namespace holgen
