#pragma once

#include "../TranslatorPlugin.h"
#include "holgen.h"

namespace holgen {
class StringSwitcher;

class LuaFieldsPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessRegistryDataField(Class &cls, const FieldDefinition &fieldDefinition);
  void ProcessStruct(Class &cls);
};

} // namespace holgen
