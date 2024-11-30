#pragma once
#include "ContainerPluginBase.h"

namespace holgen {
class ContainerGetElemPlugin : ContainerPluginBase {
public:
  using ContainerPluginBase::ContainerPluginBase;
  void Run() override;

private:
  void ProcessField(Class &cls, const ClassField &field) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const override;
  void GenerateGetElemMethods(Class &cls, const ClassField &field) const;
  void ProcessMethodVisibility(const ClassField &field, ClassMethod &method) const;
  void GenerateGetElemBodyForKeyedContainer(CodeBlock &codeBlock, const ClassField &field) const;
  void GenerateBoundsCheckForIndexedContainer(CodeBlock &codeBlock, const ClassField &field,
                                              const std::string &idxExpression) const;
  void GenerateGetElemBodyForCompositeId(CodeBlock &codeBlock, const ClassField &field,
                                         const std::string &idxExpression) const;
  void GenerateGetElemBodyForIndexedContainer(CodeBlock &codeBlock, const ClassField &field,
                                              const std::string &idxExpression) const;

  void GenerateGetElemByIdxForContainer(Class &cls, const ClassField &field,
                                        const std::string &methodName, const Type &idxType,
                                        Constness constness) const;
  void GenerateGetElemByCompositeIdForContainer(Class &cls, const ClassField &field,
                                                const std::string &methodName, Constness constness) const;
};
} // namespace holgen
