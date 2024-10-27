#pragma once

#include <string>
#include <map>
#include "../TypeInfo.h"

namespace holgen {
enum class InteropType {
  ManagedToNative,
  NativeToManaged,
  Internal,
};

class CSharpHelper {
public:
  // Returns a type string that can be used in c# to represent the provided type.
  std::string RepresentationInNative(const Type &other, const TranslatedProject &project);
  std::string RepresentationInManaged(const Type &other, const TranslatedProject &project);
  std::string Representation(const Type &other, const TranslatedProject &project,
                             InteropType interopType);
  std::string VariableRepresentation(const Type &other, const std::string &variableName,
                                     const TranslatedProject &project, InteropType interopType);
  static CSharpHelper &Get();
  std::map<std::string, std::string> CppTypeToCSharpType;

private:
  CSharpHelper();
  std::string ManagedVariableToNative(const Type &other, const std::string &variableName,
                                      const TranslatedProject &project);
  std::string NativeVariableToManaged(const Type &other, const std::string &variableName,
                                      const TranslatedProject &project);
};
} // namespace holgen
