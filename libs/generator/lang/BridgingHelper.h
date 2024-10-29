#pragma once
#include "generator/TypeInfo.h"

#include "generator/TranslatedProject.h"

namespace holgen {
class BridgingHelper {
public:
  static Type ConvertType(const TranslatedProject &project, const Type &type, bool isReturnType,
                          const DefinitionSource &definitionSource);
  static MethodArgument& AddArgument(const TranslatedProject &project,MethodBase& method,
                          const MethodArgument &arg, const DefinitionSource &definitionSource);
  static void AddAuxiliaryArguments(const TranslatedProject &project,
                                   MethodBase& method, const Type &type,
                                   const std::string &argPrefix, bool isReturnType);
};
} // namespace holgen
