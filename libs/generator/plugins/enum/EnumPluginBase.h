#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class EnumPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;

protected:
  enum class EnumOperatorReturnType {
    This,
    Result,
  };

  enum class EnumOperatorArgumentType {
    UnderlyingType,
    Class,
    Entry,
  };

  struct EnumOperator {
    std::string mOperator;
    Constness mConstness;
    EnumOperatorReturnType mReturn;
    EnumOperatorArgumentType mArgumentType;
    // bool mIntegralArgument;
  };

  void GenerateIntegralConstructor(Class &cls, const std::string &defaultValue);
  void GenerateUnderlyingType(Class &cls);
  void GenerateValueField(Class &cls);
  void GenerateClassEnum(Class &cls, const std::string &entrySuffix = "");
  void GenerateFromStringSingle(Class &cls, const std::string &methodName, Visibility visibility,
                                const std::string &valueOnFailure = "");
  void GenerateToStringSingle(Class &cls, const std::string &methodName, Visibility visibility);
  void GenerateOperator(Class &cls, const EnumOperator &op);
  void GenerateOperators(Class &cls);
  void GenerateGetEntries(Class &cls, const std::string &methodName,
                          const std::string &entrySuffix);
  void GenerateFormatter(Class &cls, bool forNestedEnum);
};
} // namespace holgen
