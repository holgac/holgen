#pragma once
#include "CSharpHelper.h"


#include "generator/TranslatedProject.h"

namespace holgen {

enum class CSharpMethodType {
  WrappedClassDelegate,
  WrappedClassCaller,
  ModuleInterfaceDelegate,
  ModuleInterfaceAbstractMethod,
};

class CSharpMethodHelper {
public:
  explicit CSharpMethodHelper(TranslatedProject &project, const Class &cls, CSharpClass &csCls,
                              CSharpMethodType methodType) :
      mProject(project), mClass(cls), mCsClass(csCls), mMethodType(methodType) {}

  CSharpMethod GenerateMethod(const ClassMethod &method);

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
  TranslatedProject &mProject;
  const Class &mClass;
  CSharpClass &mCsClass;
  CSharpMethodType mMethodType;
};

} // namespace holgen
