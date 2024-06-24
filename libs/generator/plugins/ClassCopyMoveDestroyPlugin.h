#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
class ClassCopyMoveDestroyPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  using VariantData = std::map<std::string, std::vector<ClassField *>>;
  void ProcessClass(Class &cls);
  void PopulateMethod(Class &cls, ClassMethodBase &method, VariantData &variantData, bool isMove,
                      bool returnThis);
  bool NeedsCustomCopyOperator(Class &cls);
  void PopulateMethodVariantFields(Class &cls, ClassMethodBase &method, VariantData &variantData,
                                   bool isMove);
};
} // namespace holgen
