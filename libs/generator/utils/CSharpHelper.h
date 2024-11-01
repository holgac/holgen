#pragma once

#include "NamingConvention.h"


#include <string>
#include <map>
#include <list>
#include "../TypeInfo.h"

#include "generator/TranslatedProject.h"

namespace holgen {
class Class;
enum class InteropType {
  ManagedToNative,
  NativeToManaged,
  Internal,
};

class CSharpHelper {
public:
  [[nodiscard]] bool NeedsDeleterArgument(const Type &type) const;
  [[nodiscard]] bool NeedsSizeArgument(const Type &type) const;
  [[nodiscard]] CSharpType ConvertFieldType(const Type &type, const TranslatedProject &project,
                         InteropType interopType, bool returnType) const;
  static CSharpHelper &Get();
  std::map<std::string, std::string> CppTypeToCSharpType;
  std::set<std::string> CppTypesConvertibleToCSharpArray;
  std::set<std::string> CSharpTypesSupportedByMarshalCopy;

private:
  CSharpHelper();
};
} // namespace holgen
