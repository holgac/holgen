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
  void AddRegistryDataField(Class &cls, const FieldDefinition &fieldDefinition);
  void AddRegistryDataUninitializer(Class &cls, ClassField &field);
  void AddRegistryDataInitializer(Class &cls, ClassField &field);
  void ProcessStruct(Class &cls);
  ClassMethod &GetLuaInitializer(Class &cls);
  ClassMethod &GetLuaUninitializer(Class &cls);
};

} // namespace holgen
