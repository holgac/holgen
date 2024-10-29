#pragma once

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
  bool NeedsDeleter(const Type& type);
  bool NeedsSizeArgument(const Type& type);
  CSharpType ConvertType(const Type &type, const TranslatedProject &project,
                         InteropType interopType, bool returnType);
  void AddAttributes(std::list<std::string> &attributes, const Type &type, const CSharpType &csType,
                     InteropType interopType, bool isReturnType, size_t sizeArgIndex);
  void AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments, const Type &type,
                         const std::string &argPrefix, InteropType interopType, bool isReturnValue);
  std::string StringifyPassedExtraArguments(const Type& type, const std::string& argPrefix, InteropType interopType);
  // Returns a type string that can be used in c# to represent the provided type.
  std::string RepresentationInNative(const Type &other, const Type &originalType,
                                     const TranslatedProject &project, bool prependRef = false);
  std::string RepresentationInManaged(const Type &other, const Type &originalType,
                                      const TranslatedProject &project);
  std::string Representation(const Type &other, const Type &originalType,
                             const TranslatedProject &project, InteropType interopType,
                             bool prependRef);
  std::string MarshallingInfo(const Type &other, const TranslatedProject &project,
                              InteropType interopType);
  std::string ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                   InteropType interopType, size_t sizeArgumentIdx);
  // TODO: rm
  std::string VariableRepresentation(const Type &other, const std::string &variableName,
                                     const TranslatedProject &project, InteropType interopType,
                                     bool prependRef);

  std::string VariableRepresentation(const CSharpType &type, const std::string &variableName,
                                     const TranslatedProject &project, InteropType interopType);
  static CSharpHelper &Get();
  std::map<std::string, std::string> CppTypeToCSharpType;
  std::set<std::string> CppTypesConvertibleToCSharpArray;
  std::set<std::string> CSharpTypesSupportedByMarshalCopy;

private:
  CSharpHelper();
  // TODO: rm
  std::string VariableRepresentationInNative(const Type &other, const std::string &variableName,
                                             const TranslatedProject &project, bool prependRef);
  // TODO: rm
  std::string VariableRepresentationInManaged(const Type &other, const std::string &variableName,
                                              const TranslatedProject &project);
  std::string VariableRepresentationInNative(const CSharpType &type,
                                             const std::string &variableName,
                                             const TranslatedProject &project);
  std::string VariableRepresentationInManaged(const CSharpType &type,
                                              const std::string &variableName,
                                              const TranslatedProject &project);
  std::string MarshallingInfo(const Type &other, const TranslatedProject &project);
  std::string ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                   size_t sizeArgumentIdx);
};
} // namespace holgen
