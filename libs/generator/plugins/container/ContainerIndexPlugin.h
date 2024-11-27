#pragma once
#include "ContainerPluginBase.h"

namespace holgen {
class ContainerIndexPlugin : ContainerPluginBase {
public:
  using ContainerPluginBase::ContainerPluginBase;

  void Run() override;

private:
  void ProcessField(Class &cls, const ClassField &field);
  void ProcessIndex(Class &cls, const ClassField &field, const AnnotationDefinition &annotationDefinition) const;
  void GenerateIndexField(Class &cls, const ClassField& field, const AnnotationDefinition &annotationDefinition) const;
  void GenerateIndexGetter(Class &cls, const ClassField& field, const AnnotationDefinition &annotationDefinition, Constness constness) const;
};
} // namespace holgen
