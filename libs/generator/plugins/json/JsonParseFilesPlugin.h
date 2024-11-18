#pragma once
#include "JsonPluginBase.h"

namespace holgen {
class JsonParseFilesPlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void GenerateConverterPopulators(Class &cls, ClassMethod &method);
  void GenerateParseFiles(Class &cls);
  void GenerateFilesByName(ClassMethod &method);
  void GenerateReadSelf(CodeBlock& codeBlock);
  std::vector<const ClassField*> GetProcessOrder(const Class& cls);
};
} // namespace holgen
