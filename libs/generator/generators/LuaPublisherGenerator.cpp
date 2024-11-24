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
  GenerateModulesMap(codeBlock, cls);

  CodeBlock callbacks;
  CodeBlock methods;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
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
  codeBlock.Add("");
  GenerateSubscribeToEvent(codeBlock, cls);
  codeBlock.Add("");
  GenerateUnsubscribeFromEvent(codeBlock, cls);
  codeBlock.Add("");
  codeBlock.Indent(-1);
  codeBlock.Add("}}");

  out.mBody = std::move(codeBlock);
}

void LuaPublisherGenerator::GenerateModulesMap(CodeBlock &codeBlock, const Class &cls) const {
  codeBlock.Add("---@type table[string, {}]", cls.mStruct->mMixins[0]);
  codeBlock.Add("{} = {{}},", St::LuaPublisher_ModulesField);
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

  auto listenerClass = mTranslatedProject.GetClass(cls.mStruct->mMixins.front());
  auto calledMethod = listenerClass->GetMethod(method.mName, method.mConstness);
  THROW_IF(!calledMethod,
           "Publisher {} ({}) could not find the {} method in the listener class {} ({})",
           cls.mName, cls.mStruct->mDefinitionSource, method.mName, listenerClass->mName,
           listenerClass->mStruct->mDefinitionSource);

  bool hasRetVal = method.mReturnType.mName != "void" ||
      method.mFunction->mReturnType.mType.mName == St::Lua_CustomData;
  if (hasRetVal) {
    codeBlock.Add("---@type table[string, {}]",
                  calledMethod->mReturnType.mName != "void" ? calledMethod->mReturnType.mName
                                                            : "table[any]");
    codeBlock.Add("local retVal = {{}}");
  }
  codeBlock.Add("for _, module in pairs({}.{}) do", cls.mName,
                mNamingConvention.LuaPublisherCallbacksTableName(method.mName));
  codeBlock.Indent(1);

  std::string accessor = ":";
  if (calledMethod->IsStatic(*listenerClass))
    accessor = ".";

  auto caller = std::format("module{}{}({})", accessor, method.mName, args);
  if (hasRetVal) {
    codeBlock.Add("retVal[module.name] = {}", caller);
  } else {
    codeBlock.Add("{}", caller);
  }

  codeBlock.Indent(-1);
  codeBlock.Add("end");
  if (hasRetVal) {
    codeBlock.Add("return retVal");
  }
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateRegisterSubscriberMethod(CodeBlock &codeBlock,
                                                             const Class &cls) const {
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("RegisterSubscriber = function(subscriber)");
  codeBlock.Indent(1);
  codeBlock.Add("{}[subscriber.name] = subscriber", St::LuaPublisher_ModulesField);
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
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
  codeBlock.Add("{}[subscriber.name] = nil", St::LuaPublisher_ModulesField);
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    auto tableName =
        cls.mName + "." + mNamingConvention.LuaPublisherCallbacksTableName(method.mName);
    codeBlock.Add("if subscriber.{} then", method.mName);
    codeBlock.Indent(1);

    codeBlock.Add("{0}.UnsubscribeFromEvent({0}.{1}, subscriber)", cls.mName,
                  mNamingConvention.LuaPublisherCallbacksTableName(method.mName));

    codeBlock.Indent(-1);
    codeBlock.Add("end");
  }
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateUnregisterSubscriberByNameMethod(CodeBlock &codeBlock,
                                                                     const Class &cls) const {
  codeBlock.Add("---@param subscriberName string");
  codeBlock.Add("{} = function(subscriber)", St::LuaPublisher_UnregisterSubscriberByName);
  codeBlock.Indent(1);

  codeBlock.Add("if {}[subscriber.name] then", St::LuaPublisher_ModulesField);
  codeBlock.Indent(1);
  codeBlock.Add("{}.UnregisterSubscriber({}[subscriber.name])", cls.mName,
                St::LuaPublisher_ModulesField);
  codeBlock.Indent(-1);
  codeBlock.Add("end");

  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateReloadSubscriber(CodeBlock &codeBlock, const Class &cls) const {
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("ReloadSubscriber = function(subscriber)");
  codeBlock.Indent(1);

  codeBlock.Add("if {}[subscriber.name] then", St::LuaPublisher_ModulesField);
  codeBlock.Indent(1);
  codeBlock.Add("{}.UnregisterSubscriber({}[subscriber.name])", cls.mName,
                St::LuaPublisher_ModulesField);
  codeBlock.Indent(-1);
  codeBlock.Add("end");

  codeBlock.Add("{}.RegisterSubscriber(subscriber)", cls.mName);
  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateSubscribeToEvent(CodeBlock &codeBlock, const Class &cls) const {
  codeBlock.Add("---@param eventTable table[{}]", cls.mStruct->mMixins.front());
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("SubscribeToEvent = function(eventTable, subscriber)");
  codeBlock.Indent(1);

  codeBlock.Add("{}.UnsubscribeFromEvent(eventTable, subscriber)", cls.mName);
  codeBlock.Add("table.insert(eventTable, subscriber)");

  codeBlock.Indent(-1);
  codeBlock.Add("end,");
}

void LuaPublisherGenerator::GenerateUnsubscribeFromEvent(CodeBlock &codeBlock,
                                                         const Class &cls) const {
  codeBlock.Add("---@param eventTable table[{}]", cls.mStruct->mMixins.front());
  codeBlock.Add("---@param subscriber {}", cls.mStruct->mMixins.front());
  codeBlock.Add("---@return boolean", cls.mStruct->mMixins.front());
  codeBlock.Add("UnsubscribeFromEvent = function(eventTable, subscriber)");
  codeBlock.Indent(1);

  codeBlock.Add("for i = 1, #eventTable do");
  codeBlock.Indent(1);
  codeBlock.Add("if eventTable[i] == subscriber then");
  codeBlock.Indent(1);
  // more optimal to assign last element to it, then pop?
  codeBlock.Add("table.remove(eventTable, i)");
  codeBlock.Add("return true");
  codeBlock.Indent(-1);
  codeBlock.Add("end");
  codeBlock.Indent(-1);
  codeBlock.Add("end");

  codeBlock.Add("return false");

  codeBlock.Indent(-1);
  codeBlock.Add("end");
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

bool LuaPublisherGenerator::ShouldProcess(const ClassMethod &method) const {
  if (!LuaGeneratorBase::ShouldProcess(method))
    return false;
  return method.mFunction;
}
} // namespace holgen
