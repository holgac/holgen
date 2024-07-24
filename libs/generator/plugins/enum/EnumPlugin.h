#pragma once
#include "EnumPluginBase.h"

namespace holgen {
class EnumPlugin : public EnumPluginBase {
public:
  using EnumPluginBase::EnumPluginBase;
  void Run() override;

private:
  void GenerateInvalidEntry(Class &cls);
  void GenerateGetValue(Class &cls);
  void GenerateEnumConstructor(Class &cls);
  void GenerateHash(Class &cls);
  void GenerateIntegralConstructor(Class &cls);
};
} // namespace holgen
