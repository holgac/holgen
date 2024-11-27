#pragma once
#include "ContainerPluginBase.h"


namespace holgen {
class ContainerAddElemPlugin  : ContainerPluginBase {
public:
  using ContainerPluginBase::ContainerPluginBase;
  void Run() override;

private:
  void ProcessField(Class &cls, const ClassField &field) const;
  [[nodiscard]] bool ShouldProcess(const ClassField& field) const override;
  void GenerateNextIndexField(Class &cls, const ClassField &field) const;
  void GenerateAddElemMethod(Class &cls, const ClassField &field, bool useMoveRef) const;
};
}
