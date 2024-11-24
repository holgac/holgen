#pragma once
#include "LuaGeneratorBase.h"

namespace holgen {


class LuaPublisherGenerator : public LuaGeneratorBase {
public:
  using LuaGeneratorBase::LuaGeneratorBase;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateModulesMap(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodCallbacks(CodeBlock &codeBlock, const Class &cls,
                               const ClassMethod &method) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  void GenerateRegisterSubscriberMethod(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateUnregisterSubscriberMethod(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateUnregisterSubscriberByNameMethod(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateReloadSubscriber(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateSubscribeToEvent(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateUnsubscribeFromEvent(CodeBlock &codeBlock, const Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const override;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const override;
};
} // namespace holgen
