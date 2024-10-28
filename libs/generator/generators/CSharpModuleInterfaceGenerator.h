#pragma once
#include "CSharpGeneratorBase.h"

namespace holgen {
class CSharpModuleInterfaceGenerator : public CSharpGeneratorBase {
public:
  using CSharpGeneratorBase::CSharpGeneratorBase;
  void Run(std::vector<GeneratedContent> &contents) const override;
private:
  void Generate(GeneratedContent& out, const Class& cls) const;
};
} // namespace holgen
