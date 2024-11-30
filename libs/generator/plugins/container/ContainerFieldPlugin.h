#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ContainerFieldPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessField(Class &cls, ClassField &field);
  void ProcessFieldWithCompositeId(Class &cls, const ClassField &field) const;
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
