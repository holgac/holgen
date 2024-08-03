#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class BitmapFieldPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessClass(Class &cls);
  void AddMethod(Class &cls, const ClassField &field, const Class &fieldClass,
                 const std::string &name, bool isMutator);
};
} // namespace holgen
