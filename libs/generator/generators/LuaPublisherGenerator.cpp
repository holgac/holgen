#include "core/Annotations.h"
#include "LuaPublisherGenerator.h"
#include "core/St.h"

namespace holgen {
void LuaPublisherGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (!mGeneratorSettings.IsFeatureEnabled(GeneratorFeatureFlag::Lua))
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    THROW_IF(cls.mStruct->mMixins.empty(), "Publisher {} ({}) does not inherit from a listener!",
             cls.mName, cls.mStruct->mDefinitionSource);
    auto listenerClass = mTranslatedProject.GetClass(cls.mStruct->mMixins.front());
    THROW_IF(!listenerClass || !listenerClass->mStruct,
             "Publisher {} ({}) inherits from {} which is not a valid listener!", cls.mName,
             cls.mStruct->mDefinitionSource, cls.mStruct->mMixins.front());
    Generate(contents.emplace_back(), cls);
  }
}

void LuaPublisherGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  out.mType = FileType::LuaSource;
  out.mName = std::format("{}.lua", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("-- {}", St::GenMessage);
  auto luaClassName = mNamingConvention.LuaMetatableName(cls.mName);
  codeBlock.Add("---@class {}", luaClassName);
  codeBlock.Add("{} = {{", luaClassName);
  codeBlock.Indent(1);
  CodeBlock callbacks;
  CodeBlock methods;
  for (auto &method: cls.mMethods) {
    if (!LuaGeneratorBase::ShouldProcess(method))
      continue;
    GenerateMethodCallbacks(callbacks, cls, method);
    GenerateMethod(methods, cls, method);
  }
  codeBlock.Add(std::move(callbacks));
  codeBlock.Add("");
  codeBlock.Add(std::move(methods));
  codeBlock.Add("");
  GenerateRegisterSubscriberMethod(codeBlock, cls);
  codeBlock.Add("");
  GenerateUnregisterSubscriberMethod(codeBlock, cls);
  codeBlock.Add("");
  GenerateReloadSubscriber(codeBlock, cls);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");

  out.mBody = std::move(codeBlock);
}

void LuaPublisherGenerator::GenerateMethodCallbacks(CodeBlock &codeBlock, const Class &cls,
                                                    const ClassMethod &method) const {
  codeBlock.Add("---@type [{}]", cls.mStruct->mMixins[0]);
  codeBlock.Add("{} = {{}},", mNamingConvention.LuaPublisherCallbacksTableName(method.mName));
}

void LuaPublisherGenerator::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
                                           const ClassMethod &method) const {
  auto [args, annotations] = ConstructMethodArgumentsAndAnnotations(method);
  codeBlock.Add(std::move(annotations));
  codeBlock.Add("{} = function({})", method.mName, args);
  codeBlock.Indent(1);
  codeBlock.Add("for _, module in pairs({}.{}) do", cls.mName,
                mNamingConvention.LuaPublisherCallbacksTableName(method.mName));
  codeBlock.Indent(1);

  auto listenerClass = mTranslatedProject.GetClass(cls.mStruct->mMixins.front());
  auto calledMethod = listenerClass->GetMethod(method.mName, method.mConstness);
  THROW_IF(!calledMethod,
           "Publisher {} ({}) could not find the {} method in the listener class {} ({})",
           cls.mName, cls.mStruct->mDefinitionSource, method.mName, listenerClass->mName,
           listenerClass->mStruct->mDefinitionSource);
  std::string accessor = ":";
  if (calledMethod->IsStatic(*listenerClass))
    accessor = ".";

  codeBlock.Add("module{}{}({})", accessor, method.mName, args);

  codeBlock.Indent(-1);
  codeBlock.Add("end");
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateRegisterSubscriberMethod(CodeBlock &codeBlock,
                                                             const Class &cls) const {
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("RegisterSubscriber = function(subscriber)");
  codeBlock.Indent(1);
  for (auto &method: cls.mMethods) {
    if (!LuaGeneratorBase::ShouldProcess(method))
      continue;
    auto tableName =
        cls.mName + "." + mNamingConvention.LuaPublisherCallbacksTableName(method.mName);
    codeBlock.Add("if subscriber.{} then", method.mName);
    codeBlock.Indent(1);
    codeBlock.Add("table.insert({}, subscriber)", tableName);
    codeBlock.Indent(-1);
    codeBlock.Add("end");
  }
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateUnregisterSubscriberMethod(CodeBlock &codeBlock,
                                                               const Class &cls) const {
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("UnregisterSubscriber = function(subscriber)");
  codeBlock.Indent(1);
  for (auto &method: cls.mMethods) {
    if (!LuaGeneratorBase::ShouldProcess(method))
      continue;
    auto tableName =
        cls.mName + "." + mNamingConvention.LuaPublisherCallbacksTableName(method.mName);
    codeBlock.Add("if subscriber.{} then", method.mName);
    codeBlock.Indent(1);
    codeBlock.Add("for i = 1, #{} do", tableName);
    codeBlock.Indent(1);
    codeBlock.Add("if {}[i] == subscriber then", tableName);
    codeBlock.Indent(1);
    codeBlock.Add("table.remove({}, i)", tableName);
    codeBlock.Add("break", tableName);
    codeBlock.Indent(-1);
    codeBlock.Add("end");
    codeBlock.Indent(-1);
    codeBlock.Add("end");
    codeBlock.Indent(-1);
    codeBlock.Add("end");
  }
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateReloadSubscriber(CodeBlock &codeBlock, const Class &cls) const {

  codeBlock.Add("---@param oldSubscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("---@param newSubscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("---@return {}", cls.mStruct->mMixins.front());
  codeBlock.Add("ReloadSubscriber = function(oldSubscriber, newSubscriber)");
  codeBlock.Indent(1);

  codeBlock.Add("if oldSubscriber then");
  codeBlock.Indent(1);
  codeBlock.Add("{}.UnregisterSubscriber(oldSubscriber)", cls.mName);
  codeBlock.Indent(-1);
  codeBlock.Add("end");

  codeBlock.Add("{}.RegisterSubscriber(newSubscriber)", cls.mName);
  codeBlock.Add("return newSubscriber", cls.mName);

  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

bool LuaPublisherGenerator::ShouldProcess(const Class &cls) const {
  if (!LuaGeneratorBase::ShouldProcess(cls))
    return false;
  if (!cls.mStruct ||
      !cls.mStruct->GetMatchingAttribute(Annotations::LuaFuncTable,
                                         Annotations::LuaFuncTable_Publisher)) {
    return false;
  }
  return true;
}
} // namespace holgen
