#pragma once
#include "../TranslatorPlugin.h"

namespace holgen {
class JsonDumpPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GenerateForField(Class &cls, CodeBlock &codeBlock, const ClassField &field, const std::string& fieldName);
  bool ShouldProcess(const ClassField &field);
  bool ShouldProcess(const Class &cls);
};
} // namespace holgen
