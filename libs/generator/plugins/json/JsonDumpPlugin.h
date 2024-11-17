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
};
} // namespace holgen
