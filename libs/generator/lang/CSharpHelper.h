#pragma once

#include <string>
#include <map>
#include "../TypeInfo.h"

namespace holgen {
class Class;
enum class InteropType {
  ManagedToNative,
  NativeToManaged,
  Internal,
};

class CSharpHelper {
public:
  // Returns a type string that can be used in c# to represent the provided type.
  std::string RepresentationInNative(const Type &other, const TranslatedProject &project,
                                     bool prependRef = false);
  std::string RepresentationInManaged(const Type &other, const TranslatedProject &project);
  std::string Representation(const Type &other, const TranslatedProject &project,
                             InteropType interopType, bool prependRef);
  std::string VariableRepresentation(const Type &other, const std::string &variableName,
                                     const TranslatedProject &project, InteropType interopType,
                                     bool prependRef);
  static CSharpHelper &Get();
  std::map<std::string, std::string> CppTypeToCSharpType;

private:
  CSharpHelper();
  std::string VariableRepresentationInNative(const Type &other, const std::string &variableName,
                                             const TranslatedProject &project, bool prependRef);
  std::string VariableRepresentationInManaged(const Type &other, const std::string &variableName,
                                              const TranslatedProject &project);
};
} // namespace holgen
