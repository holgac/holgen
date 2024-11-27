#pragma once


#include "../TranslatorPlugin.h"
namespace holgen {
class CompositeIdStructPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  [[nodiscard]] bool ShouldProcess(const Class& cls) const;
  void Process(Class& cls) const;
  void ValidateStruct(const Class& cls) const;
  void AddIdGetterMethod(Class& cls) const;
  void AddIsValidMethod(Class& cls) const;
};

}
