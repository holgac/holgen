#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class DotNetDeferredDeleterPlugin  : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void GeneratePerformManaged(const Class& cls, CSharpClass& csCls);
  void GeneratePerformManagedArray(const Class& cls, CSharpClass& csCls);
};
}
