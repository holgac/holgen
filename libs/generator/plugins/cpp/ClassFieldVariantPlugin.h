#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class ClassFieldVariantPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
  void ProcessVariantField(Class &cls, const FieldDefinition &fieldDefinition);
  void ProcessVariantType(Class &cls, const std::string &typeFieldName);
  enum class VariantTypeProcessType {
    Setter,
    Resetter,
    Mover,
    Copier,
  };
  void ProcessVariantTypeCommon(Class &cls, const std::string &typeFieldName, MethodBase &method,
                                VariantTypeProcessType processType);
};
} // namespace holgen
