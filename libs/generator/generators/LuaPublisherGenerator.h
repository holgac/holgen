#pragma once
#include "LuaGeneratorBase.h"

namespace holgen {


class LuaPublisherGenerator : public LuaGeneratorBase {
public:
  using LuaGeneratorBase::LuaGeneratorBase;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateMethodCallbacks(CodeBlock &codeBlock, const Class &cls,
                               const ClassMethod &method) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  void GenerateRegisterSubscriberMethod(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateUnregisterSubscriberMethod(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateReloadSubscriber(CodeBlock &codeBlock, const Class &cls) const;
  bool ShouldProcess(const Class &cls) const override;
};
} // namespace holgen
