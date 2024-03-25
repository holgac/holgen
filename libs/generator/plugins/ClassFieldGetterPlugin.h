#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassFieldGetterPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  private:
    void ProcessRefField(Class &generatedClass, ClassField &generatedField,
                         const FieldDefinition &fieldDefinition) const;
  };
}