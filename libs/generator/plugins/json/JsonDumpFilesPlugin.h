#pragma once
#include "JsonPluginBase.h"

namespace holgen {
class JsonDumpFilesPlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void GenerateDumpFiles(Class &cls);
  void GenerateRecreateDirectory(CodeBlock& codeBlock);
  void GenerateDumpSelf(CodeBlock &codeBlock);
  void GenerateDumpContainerField(const ClassField &field, CodeBlock &codeBlock);
};
} // namespace holgen
