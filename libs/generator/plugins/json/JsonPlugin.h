#pragma once

#include <map>
#include <set>
#include <string>
#include "../TranslatorPlugin.h"

namespace holgen {
class JsonPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GenerateParseJson(Class &cls);
  void GenerateParseJsonForField(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                 const std::string &varName);
  void GenerateParseJsonForFunction(Class &cls, CodeBlock &codeBlock,
                                    const ClassMethod &luaFunction);
  bool IsVariantTypeField(const Class &cls, const ClassField &field,
                          const std::string **rawName = nullptr);
  void GenerateParseJsonJsonConvert(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                    const std::string &varName);
  void GenerateParseJsonVariantType(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                    const std::string &varName, const std::string &rawFieldName);
  void GenerateParseJsonVariant(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                const std::string &varName);
};
} // namespace holgen
