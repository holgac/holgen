#pragma once
#include "../TranslatorPlugin.h"
#include "JsonPluginBase.h"

namespace holgen {
class JsonDumpPlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GenerateForField(CodeBlock &codeBlock, const ClassField &field,
                        const std::string &fieldName);
  void ProcessStructFields(Class &cls, CodeBlock& codeBlock);
  void ProcessStructVariantFields(Class &cls, CodeBlock& codeBlock);
};
} // namespace holgen
