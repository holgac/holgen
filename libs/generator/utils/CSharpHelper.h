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
  bool NeedsDeleter(const Type &type) const;
  bool NeedsSizeArgument(const Type &type) const;
  CSharpType ConvertType(const Type &type, const TranslatedProject &project,
                         InteropType interopType, bool returnType) const;
  void AddAttributes(std::list<std::string> &attributes, const Type &type, const CSharpType &csType,
                     InteropType interopType, bool isReturnType, size_t sizeArgIndex) const;
  void AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments, const Type &type,
                             const std::string &argPrefix, InteropType interopType,
                             bool isReturnValue) const;
  std::string StringifyPassedExtraArguments(const Type &type, const std::string &argPrefix,
                                            InteropType interopType) const;
  // Returns a type string that can be used in c# to represent the provided type.
  std::string RepresentationInNative(const Type &other, const Type &originalType,
                                     const TranslatedProject &project,
                                     bool prependRef = false) const;
  std::string RepresentationInManaged(const Type &other, const Type &originalType,
                                      const TranslatedProject &project) const;
  std::string MarshallingInfo(const Type &other, const TranslatedProject &project,
                              InteropType interopType) const;
  std::string ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                   InteropType interopType, size_t sizeArgumentIdx) const;
  std::string VariableRepresentation(const CSharpType &type, const std::string &variableName,
                                     const TranslatedProject &project,
                                     InteropType interopType) const;
  [[nodiscard]] CSharpMethod CreateMethod(const TranslatedProject &project, const Class &cls,
                                          const ClassMethod &method, InteropType argsInteropType,
                                          InteropType returnTypeInteropType, bool addThisArgument,
                                          bool ignoreAuxiliaries) const;
  [[nodiscard]] CSharpConstructor CreateConstructor(const TranslatedProject &project,
                                                    const Class &cls, const ClassMethod &method,
                                                    InteropType interopType) const;
  static CSharpHelper &Get();
  std::map<std::string, std::string> CppTypeToCSharpType;
  std::set<std::string> CppTypesConvertibleToCSharpArray;
  std::set<std::string> CSharpTypesSupportedByMarshalCopy;

  void GenerateWrapperCall(CodeBlock &codeBlock, const TranslatedProject &project,
                           InteropType argsInteropType, InteropType returnTypeInteropType,
                           const std::string &methodToCall, const CSharpClass &cls,
                           const ClassMethod &method, const CSharpMethodBase &csMethod,
                           bool addThisArgument) const;
  void GenerateWrapperCallReturningVoid(CodeBlock &codeBlock, const TranslatedProject &project,
                                        InteropType interopType, const std::string &methodToCall,
                                        const CSharpClass &cls, const ClassMethod &method,
                                        const CSharpMethodBase &csMethod,
                                        bool addThisArgument) const;
  void GenerateWrapperCallReturningArray(
      CodeBlock &codeBlock, const TranslatedProject &project, InteropType argsInteropType,
      InteropType returnTypeInteropType, const std::string &methodToCall, const CSharpClass &cls,
      const ClassMethod &method, const CSharpMethodBase &csMethod, bool addThisArgument) const;
  void GenerateWrapperCallReturningValue(CodeBlock &codeBlock, const TranslatedProject &project,
                                         InteropType argsInteropType,
                                         const std::string &methodToCall, const CSharpClass &cls,
                                         const ClassMethod &method,
                                         const CSharpMethodBase &csMethod,
                                         bool addThisArgument) const;
  void GenerateWrapperCallReturningClass(CodeBlock &codeBlock, const TranslatedProject &project,
                                         InteropType argsInteropType,
                                         InteropType returnTypeInteropType,
                                         const std::string &methodToCall, const CSharpClass &cls,
                                         const ClassMethod &method,
                                         const CSharpMethodBase &csMethod, bool addThisArgument,
                                         const Class &returnType) const;
  [[nodiscard]] std::string
      ConstructWrapperCall(const CSharpClass &cls, const TranslatedProject &project,
                           InteropType interopType, const std::string &methodToCall,
                           const MethodBase &method, const CSharpMethodBase &csMethod,
                           bool addThisArgument, bool hasSizeArg, bool hasDeleterArg) const;

  [[nodiscard]] std::string ConstructMethodArguments(const CSharpClass &cls,
                                                     const TranslatedProject &project,
                                                     const MethodBase &method,
                                                     const CSharpMethodBase &csMethod,
                                                     InteropType interopType, bool addThisArgument,
                                                     bool hasSizeArg, bool hasDeleterArg) const;

  [[nodiscard]] std::string GetWrapperTargetInWrappedClass(const CSharpClass &cls,
                                                           const NamingConvention &naming,
                                                           const std::string &methodName) const;

private:
  CSharpHelper();
  std::string VariableRepresentationInNative(const CSharpType &type,
                                             const std::string &variableName,
                                             const TranslatedProject &project) const;
  std::string VariableRepresentationInManaged(const CSharpType &type,
                                              const std::string &variableName,
                                              const TranslatedProject &project) const;
  std::string MarshallingInfo(const Type &other, const TranslatedProject &project) const;
  std::string ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                   size_t sizeArgumentIdx) const;
  void PopulateArguments(const TranslatedProject &project, std::list<CSharpMethodArgument> &out,
                         const std::list<MethodArgument> &arguments, InteropType interopType,
                         bool ignoreAuxiliaries) const;
};
} // namespace holgen
