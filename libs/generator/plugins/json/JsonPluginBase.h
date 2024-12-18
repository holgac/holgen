#pragma once
#include "../TranslatorPlugin.h"

namespace holgen {
class JsonPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;

  JsonPluginBase(TranslatedProject &translatedProject,
                 const TranslatorSettings &translatorSettings);

protected:
  [[nodiscard]] bool ShouldProcess(const ClassField &field, bool isVariantType) const;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const;
  [[nodiscard]] bool IsContainerOfDataManager(const Class &cls, const ClassField &field) const;
  void GenerateOnDataLoadCalls(const Class& cls, CodeBlock& codeBlock);
  std::string mLuaStateArgument;
};
} // namespace holgen
