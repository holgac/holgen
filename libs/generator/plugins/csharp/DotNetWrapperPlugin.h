#pragma once

#include "../TranslatorPlugin.h"
#include "generator/utils/CSharpHelper.h"

namespace holgen {
class DotNetWrapperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void Process(Class &cls, CSharpClass& csCls) const;
  void ProcessConstructors(const Class &cls, CSharpClass &csCls) const;
  void ProcessMethods(const Class &cls, CSharpClass &csCls) const;
  void ProcessMethodPointers(const Class &cls, CSharpClass &csCls) const;
  void ProcessProxyFields(const Class &cls, CSharpClass& csCls) const;
  void ProcessProxy(const Class &cls, CSharpClass &csCls) const;
  void ProcessMirror(const Class &cls, CSharpClass &csCls) const;
  void ProcessInitializer(const Class &cls, CSharpClass &csCls) const;
  void ProcessInitializerArguments(const Class &cls, const CSharpClass &csCls,
                                   CSharpMethod &csMethod) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const ClassConstructor &ctor) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] bool IsStaticClass(const Class &cls) const;
  void GenerateConstructorWrapperCall(CodeBlock &codeBlock, const Class &cls,
                                      const CSharpClass &csCls, const ClassMethod &method,
                                      const CSharpMethodBase &csMethod) const;
  [[nodiscard]] const ClassMethod* GetGetter(const Class& cls, const ClassField& field) const;
  [[nodiscard]] const ClassMethod* GetSetter(const Class& cls, const ClassField& field) const;
};
} // namespace holgen
