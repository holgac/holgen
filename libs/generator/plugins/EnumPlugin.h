#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class EnumPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void GenerateCommon(Class &cls);
  void GenerateOperators(Class &cls);
  void GenerateInvalidEntry(Class &cls);
  void GenerateClassEnum(Class &cls);
  void GenerateFromString(Class &cls);
  void GenerateToString(Class &cls);
  void GenerateGetEntries(Class &cls);
  void GenerateGetValue(Class &cls);
  void GenerateHash(Class &cls);
  void GenerateFormatter(Class &cls, bool forNestedEnum);
  void GenerateIntegralConstructor(Class &cls);
  void GenerateEnumConstructor(Class &cls);
};
} // namespace holgen
