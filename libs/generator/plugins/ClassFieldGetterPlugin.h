#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassFieldGetterPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  private:
    void ProcessRefField(Class &cls, ClassField &field) const;
    void ProcessField(Class &cls, ClassField &field, bool isConst) const;
  };
}
