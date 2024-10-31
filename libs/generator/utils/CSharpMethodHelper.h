#pragma once
#include "CSharpHelper.h"


#include "generator/TranslatedProject.h"

namespace holgen {

enum class CSharpMethodType {
  WrappedClassDelegate,
  WrappedClassCallerMethod,
  WrappedClassCallerConstructor,
  ModuleInterfaceDelegate,
  ModuleInterfaceAbstractMethod,
};

class CSharpMethodHelper {
public:
  explicit CSharpMethodHelper(TranslatedProject &project, const Class &cls,
                              const CSharpClass &csCls, const NamingConvention &naming,
                              CSharpMethodType methodType) :
      mProject(project), mClass(cls), mCsClass(csCls), mNaming(naming), mMethodType(methodType) {}

  CSharpMethod GenerateMethod(const ClassMethod &method);
  CSharpConstructor GenerateConstructor(const ClassMethod &method);

private:
  void SetMethodProperties(const ClassMethod &method, CSharpMethod &csMethod);
  CSharpType ConvertReturnType(const Type &type);
  CSharpType ConvertArgumentType(const Type &type);
  [[nodiscard]] std::string GetMethodName(const std::string &rawName);
  bool ShouldAddThisArgument(const ClassMethod &method);
  void PopulateArguments(std::list<CSharpMethodArgument> &out,
                         const std::list<MethodArgument> &arguments);
  void AddAttributes(std::list<std::string> &attributes, const Type &type, const CSharpType &csType,
                     bool isReturnType, size_t sizeArgIndex);
  bool ShouldUseRefArgument(const Type &type, const CSharpType &csType);
  void AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments, const Type &type,
                             const std::string &argPrefix, bool isReturnValue);
  bool ShouldHaveSizeArgument(const Type &returnType);
  bool ShouldHaveDeleter(const Type &returnType);
  // TODO: rename to ConstructWrappedMethodArguments
  [[nodiscard]] std::string ConstructMethodArguments(const ClassMethod &method,
                                                     const CSharpMethodBase &csMethod);
  [[nodiscard]] std::string VariableRepresentation(const CSharpType &type,
                                                   const std::string &variableName);
  [[nodiscard]] std::string
      GetWrapperTargetInWrappedClass(const std::string &wrappedMethodName) const;
  void GenerateMethodBody(const ClassMethod &method, CSharpMethodBase &csMethod);
  void GenerateMethodBodyForConstructor(const ClassMethod &method, CSharpMethodBase &csMethod);
  [[nodiscard]] std::string ConstructWrapperCall(const std::string &methodToCall,
                                                 const ClassMethod &method,
                                                 const CSharpMethodBase &csMethod);
  TranslatedProject &mProject;
  const Class &mClass;
  const CSharpClass &mCsClass;
  const NamingConvention &mNaming;
  CSharpMethodType mMethodType;
};

} // namespace holgen
