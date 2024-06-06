#pragma once
#include "EnumPluginBase.h"

namespace holgen {
class BitmapPlugin : public EnumPluginBase {
public:
  using EnumPluginBase::EnumPluginBase;
  void Run() override;

private:
  void GenerateClassEnumForEntryValues(Class &cls);
  void GenerateGetValue(Class &cls);
  void GenerateFromString(Class &cls);
  void GenerateToString(Class &cls);
  void GenerateBitmapOperators(Class &cls);
  void GenerateHasMethod(Class &cls, bool forNestedEnum);
};
} // namespace holgen
