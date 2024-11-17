#pragma once

#include <map>
#include <set>
#include <string>
#include "JsonPluginBase.h"

namespace holgen {
class JsonParsePlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void GenerateParseJsonForLuaFuncTable(Class &cls);
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GenerateParseJson(Class &cls);
  void GenerateParseJsonFromArray(Class &cls, CodeBlock &methodBody);
  void GenerateParseJsonFromObject(Class &cls, CodeBlock &methodBody);
  void GenerateParseJsonFields(Class &cls, CodeBlock &codeBlock);
  void GenerateParseJsonForField(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                 const std::string &varName);
  void GenerateParseJsonForFunction(Class &cls, CodeBlock &codeBlock,
                                    const ClassMethod &luaFunction, const std::string &varName);
  const AnnotationDefinition *GetConvertElemAnnotation(const FieldDefinition *field);
  const AnnotationDefinition *GetConvertKeyAnnotation(const FieldDefinition *field);
  void GenerateParseJsonJsonConvert(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                    const std::string &varName);
  void GenerateParseJsonVariantType(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                    const std::string &varName, const std::string &rawFieldName);
  void GenerateParseJsonJsonConvertKeyElem(Class &cls, CodeBlock &codeBlock,
                                           const ClassField &field, const std::string &varName,
                                           const AnnotationDefinition *convertElemAnnotation,
                                           const AnnotationDefinition *convertKeyAnnotation);
  void GenerateParseJsonJsonConvertKey(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                       const std::string &varName,
                                       const AnnotationDefinition *convertKeyAnnotation);
  void GenerateParseJsonJsonConvertElem(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                        const std::string &varName,
                                        const AnnotationDefinition *convertElemAnnotation);
  void GenerateParseJsonJsonConvertField(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                         const std::string &varName);
  void GenerateParseJsonVariant(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                const std::string &varName);
  void GenerateSwitcherLoop(CodeBlock &methodBody, CodeBlock &&codeBlock);
};
} // namespace holgen
