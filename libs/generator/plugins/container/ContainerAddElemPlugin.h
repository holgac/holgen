#pragma once
#include "ContainerPluginBase.h"

namespace holgen {
class ContainerAddElemPlugin : ContainerPluginBase {
public:
  using ContainerPluginBase::ContainerPluginBase;
  void Run() override;

private:
  void ProcessField(Class &cls, const ClassField &field) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const override;
  void GenerateNextIndexField(Class &cls, const ClassField &field) const;
  void GenerateAddElemMethod(Class &cls, const ClassField &field, bool useMoveRef) const;
  void GenerateIndexInserterAndValidators(const Class &underlyingClass, const ClassField &field,
                                          CodeBlock &indexInserters, CodeBlock &validators) const;

  void GenerateAddElemMethodBody(const Class &cls, const Class *underlyingClass,
                                 ClassMethod &method, const ClassField &field, bool useMoveRef)const;

  void GenerateIdAssigner(CodeBlock &codeBlock, const ClassField &field,
                                   const ClassField &underlyingIdField) const;
};
} // namespace holgen
