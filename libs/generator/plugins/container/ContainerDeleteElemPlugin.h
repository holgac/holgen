#pragma once
#pragma once
#include "ContainerPluginBase.h"

namespace holgen {
class ContainerDeleteElemPlugin : ContainerPluginBase {
public:
  using ContainerPluginBase::ContainerPluginBase;
  void Run() override;

private:
  void ProcessField(Class &cls, const ClassField &field) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const override;
  void ProcessMethodVisibility(const ClassField &field, ClassMethod &method) const;
  void ConstructMethodBody(const ClassField &field, ClassMethod &method) const;
};
} // namespace holgen
