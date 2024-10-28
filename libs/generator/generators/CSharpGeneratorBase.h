#pragma once
#include "Generator.h"
#include "generator/lang/CSharpHelper.h"

namespace holgen {
class CSharpGeneratorBase : public Generator {
public:
  using Generator::Generator;

protected:
  void GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method,
                              bool isStatic) const;
  [[nodiscard]] std::string ConstructMethodArguments(const Class &cls, const MethodBase &method,
                                                     InteropType interopType,
                                                     bool addThisArgument) const;
  [[nodiscard]] std::string ConstructMethodSignatureArguments(const Class &cls,
                                                              const MethodBase &method,
                                                              InteropType interopType,
                                                              bool addThisArgument) const;
  void GenerateAbstractMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method,
                              bool isStatic, InteropType interopType) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  [[nodiscard]] std::string ConstructWrapperCall(const Class &cls, const MethodBase &method,
                                                 const std::string &methodName,
                                                 bool addThisArgument) const;
};
} // namespace holgen
