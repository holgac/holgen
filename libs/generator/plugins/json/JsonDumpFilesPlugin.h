#pragma once
#include "JsonPluginBase.h"

namespace holgen {
class JsonDumpFilesPlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void GenerateDumpFiles(Class &cls);
  void GenerateDumpSelf(CodeBlock &codeBlock);
  void GenerateDumpToFile(CodeBlock &codeBlock, const std::string &fileName);
  void GenerateDumpContainerField(const ClassField &field, CodeBlock &codeBlock);
};
} // namespace holgen
