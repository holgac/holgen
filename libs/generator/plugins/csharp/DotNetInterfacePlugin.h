#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class DotNetInterfacePlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void GenerateForCpp();
  Class &GenerateClass();
  void GenerateConstructor(Class &cls);
  void GenerateDestructor(Class &cls);
  void GenerateField(Class &cls);
  void GenerateFieldGetter(Class &cls);
  void GenerateFreeMethod(Class &cls);
  void GenerateFreeMethodPtr(Class &cls);
  void GenerateForCSharp();
  CSharpClass &GenerateCSharpClass();
  void GenerateCSharpFreeMethod(CSharpClass &cls);
};

} // namespace holgen
