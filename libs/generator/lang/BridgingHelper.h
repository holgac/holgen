#pragma once
#include "generator/TypeInfo.h"

namespace holgen {
class BridgingHelper {
public:
  static Type ConvertType(const TranslatedProject &project, const Type &type, bool isReturnType,
                          const DefinitionSource &definitionSource);
};
} // namespace holgen
