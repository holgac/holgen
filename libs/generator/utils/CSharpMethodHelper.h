#pragma once
#include "CSharpHelper.h"


#include "generator/TranslatedProject.h"

namespace holgen {

enum class CSharpMethodType {
  WrappedClassDelegate,
  WrappedClassCallerMethod,
  WrappedClassCallerConstructor,
  InterfaceClassAbstractMethod,
  InterfaceClassMethodDelegate,
  InterfaceClassMethodCaller,
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
  bool ShouldPassSizeArgument(const Type &returnType);
  bool ShouldHaveDeleterArgument(const Type &returnType);
  bool ShouldPassDeleterArgument(const Type &returnType);
  bool ShouldPassThisArgument(const ClassMethod &method);
  // TODO: rename to ConstructWrappedMethodArguments
  [[nodiscard]] std::string ConstructMethodArguments(const ClassMethod &method,
                                                     const CSharpMethodBase &csMethod);
  [[nodiscard]] std::string VariableRepresentation(const Type &type,
                                                   const std::string &variableName);
  [[nodiscard]] std::string GetWrapperTargetInWrappedClass(const ClassMethod &method);
  void GenerateMethodBody(const ClassMethod &method, CSharpMethodBase &csMethod);
  void GenerateMethodBodyForConstructor(const ClassMethod &method, CSharpMethodBase &csMethod);
  void GenerateMethodBodyForMethod(const ClassMethod &method, CSharpMethodBase &csMethod);
  void GenerateMethodBodyForMethodReturningVoid(const ClassMethod &method,
                                                CSharpMethodBase &csMethod);
  void GenerateMethodBodyForMethodReturningClass(const ClassMethod &method,
                                                 CSharpMethodBase &csMethod, const Class &cls);
  void GenerateMethodBodyForMethodReturningArray(const ClassMethod &method,
                                                 CSharpMethodBase &csMethod);
  void GenerateMethodBodyForMethodReturningValue(const ClassMethod &method,
                                                 CSharpMethodBase &csMethod);
  [[nodiscard]] std::string ConstructWrapperCall(const std::string &methodToCall,
                                                 const ClassMethod &method,
                                                 const CSharpMethodBase &csMethod);
  [[nodiscard]] std::string PassAuxiliaryArguments(const Type &type, const std::string &argPrefix);
  TranslatedProject &mProject;
  const Class &mClass;
  const CSharpClass &mCsClass;
  const NamingConvention &mNaming;
  CSharpMethodType mMethodType;
};

} // namespace holgen
