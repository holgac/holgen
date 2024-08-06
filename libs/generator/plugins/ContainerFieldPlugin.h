#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ContainerFieldPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessField(Class &cls, ClassField &field);
  void ProcessIndex(Class &cls, const ClassField &field,
                    const AnnotationDefinition &annotationDefinition);
  void GenerateIndexGetter(Class &generatedClass, const ClassField &field,
                           const AnnotationDefinition &annotationDefinition);
  void GenerateNextIndexField(Class &cls, const ClassField &field);
  void GenerateAddElem(Class &cls, const ClassField &field, bool useMoveRef);
  void GenerateHasElem(Class &cls, const ClassField &field);
  void GenerateGetElem(Class &cls, const ClassField &field);
  void GenerateGetCount(Class &cls, const ClassField &field);
  void GenerateDeleteElem(Class &cls, const ClassField &field);
  void GenerateSetElem(Class &cls, const ClassField &field);
  bool CanImplementHasElem(Class &generatedClass, const ClassField &field);
  bool CanImplementDeleteElem(Class &generatedClass, const ClassField &field);
  bool CanImplementSetElem(Class &generatedClass, const ClassField &field);
};
} // namespace holgen
