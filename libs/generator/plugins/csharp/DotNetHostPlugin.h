#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class DotNetHostPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  Class &CreateClass();
  void PopulateClass(Class &cls);
  void CreateDestructor(Class &cls);
  void CreateInitializeMethod(Class &cls);
  void CreateLoadLibraryMethod(Class &cls);
  void CreateFreeLibraryMethod(Class &cls);
  void CreateLoadFunctionMethod(Class &cls);
  void CreateLoadDelegateMethod(Class &cls);
  void GenerateLoadDelegatesMethod(CodeBlock &codeBlock);
  void CreateLoadBaseModuleMethod(Class &cls);
  void PopulateForModule(Class &cls, const Class &moduleCls);
  void CreateInitializeHolgenMethod(Class &cls);
  void CreateLoadModuleMethod(Class &cls, const Class &moduleCls);
  void GenerateInitializeClass(Class &cls, CodeBlock &codeBlock, const Class &projectCls);
  [[nodiscard]] bool ShouldInitializeClass(const Class &cls);
  [[nodiscard]] bool ShouldInitializeCFunction(const CFunction &function);
  [[nodiscard]] std::string LibraryHandleField() const;
  [[nodiscard]] std::string HostfxrMethodFieldName(const std::string &method) const;
  [[nodiscard]] std::string HostfxrMethodTypedefName(const std::string &method) const;

  static constexpr const std::array HostfxrMethods{"initialize_for_runtime_config",
                                                   "get_runtime_delegate", "close"};

  static constexpr std::array<const char *, 2> HostfxrDelegates{
      "load_assembly", "get_function_pointer"};
};
} // namespace holgen
