#pragma once

#include <string>
#include <map>
#include "../TypeInfo.h"

namespace holgen {
class CSharpHelper {
public:
  // Returns a type string that can be used in c# to represent the provided type.
  std::string TypeRepresentation(const Type &other, const TranslatedProject &project);
  static CSharpHelper& Get();
  std::map<std::string, std::string> CppTypeToCSharpType;
private:
  CSharpHelper();
};
} // namespace holgen
