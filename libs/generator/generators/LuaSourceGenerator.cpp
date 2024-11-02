#include "LuaSourceGenerator.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
void LuaSourceGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (!mGeneratorSettings.IsFeatureEnabled(GeneratorFeatureFlag::Lua))
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void LuaSourceGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  auto headerHeaders = PrepareIncludes(cls, true);
  auto headers = PrepareIncludes(cls, false);
  headers.Subtract(headerHeaders);

  out.mType = FileType::LuaSource;
  out.mName = std::format("{}.lua", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("-- {}", St::GenMessage);
  codeBlock.Add("---@diagnostic disable: missing-return");
  if (cls.mStruct) {
    GenerateClassDefinition(codeBlock, cls);
  } else if (cls.mEnum)
    GenerateEnum(codeBlock, cls);

  GenerateMethods(codeBlock, cls);

  if (cls.mStruct) {
    auto globalAttribute =
        cls.mStruct->GetMatchingAttribute(Annotations::Lua, Annotations::Lua_Global);
    if (globalAttribute) {
      codeBlock.Add("");
      codeBlock.Add("---@type {}", ToLuaType(Type{cls.mName}));
      codeBlock.Add("{} = nil", globalAttribute->mValue.mName);
    }
  }
  out.mBody = std::move(codeBlock);
}

void LuaSourceGenerator::GenerateEnum(CodeBlock &codeBlock, const Class &cls) const {
  auto entryEnum = cls.GetNestedEnum("Entry");
  CodeBlock entriesCodeBlock;
  std::stringstream aliasStatement;
  bool isFirst = true;
  for (auto &entry: entryEnum->mEntries) {
    if (isFirst)
      isFirst = false;
    else
      aliasStatement << "|";
    aliasStatement << entry.mValue;
    entriesCodeBlock.Add("{} = {},", entry.mName, entry.mValue);
  }

  codeBlock.Add("---@alias {} {}", cls.mName, aliasStatement.str());
  codeBlock.Add("local {} = {{", cls.mName);
  codeBlock.Indent(1);
  codeBlock.Add(std::move(entriesCodeBlock));
  if (cls.mEnum->mType == EnumDefinitionType::Enum)
    codeBlock.Add("Invalid = {},", cls.mEnum->mInvalidValue);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void LuaSourceGenerator::GenerateClassDefinition(CodeBlock &codeBlock, const Class &cls) const {
  auto luaClassName = cls.mName + St::LuaMetatableSuffix;
  codeBlock.Add("---@class {}", luaClassName);
  GenerateFields(codeBlock, cls);
  codeBlock.Add("local {} = {{}}", luaClassName);
}

void LuaSourceGenerator::GenerateFields(CodeBlock &codeBlock, const Class &cls) const {
  for (auto &field: cls.mFields) {
    if (!ShouldProcess(field))
      continue;
    GenerateField(codeBlock, field);
  }
}

void LuaSourceGenerator::GenerateField(CodeBlock &codeBlock, const ClassField &field) const {
  if (field.mField && field.mField->mType.mName == "Ref") {
    codeBlock.Add("---@field {}Id {}", field.mField->mName, ToLuaType(field.mType));
    codeBlock.Add("---@field {} {}", field.mField->mName,
                  ToLuaType(Type{field.mField->mType.mTemplateParameters.front().mName}));
  } else {
    codeBlock.Add("---@field {} {}", field.mField->mName, ToLuaType(field.mType));
  }
}

void LuaSourceGenerator::GenerateMethods(CodeBlock &codeBlock, const Class &cls) const {
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    codeBlock.Add("");
    GenerateMethod(codeBlock, cls, method);
  }
}

void LuaSourceGenerator::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
                                        const ClassMethod &method) const {
  std::stringstream argsStr;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    codeBlock.Add("---@param {} {}", arg.mName, ToLuaType(arg.mType));
    if (isFirst)
      isFirst = false;
    else
      argsStr << ", ";
    argsStr << arg.mName;
  }
  if (method.mReturnType != Type{"void"})
    codeBlock.Add("---@return {}", ToLuaType(method.mReturnType));
  codeBlock.Add("function {}{}:{}({}) end", cls.mName, St::LuaMetatableSuffix, method.mName,
                argsStr.str());
}

bool LuaSourceGenerator::ShouldProcess(const Class &cls) const {
  if (!cls.mStruct && !cls.mEnum)
    return false;
  if (cls.mStruct &&
      (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
       cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua))) {
    return false;
  }
  if (cls.mEnum &&
      (cls.mEnum->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
       cls.mEnum->GetMatchingAttribute(Annotations::No, Annotations::No_Lua))) {
    return false;
  }
  return true;
}

bool LuaSourceGenerator::ShouldProcess(const ClassField &field) const {
  if (!field.mField ||
      field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Lua)) {
    return false;
  }
  return true;
}

bool LuaSourceGenerator::ShouldProcess(const ClassMethod &method) const {
  return method.mExposeToLua;
}

std::string LuaSourceGenerator::ToLuaType(const Type &type) const {
  auto &ti = TypeInfo::Get();
  if (type.mName == "void" && type.mType == PassByType::Pointer)
    return "userdata";
  if (type.mName == "bool")
    return "bool";
  if (ti.IntegralTypes.contains(type.mName) || ti.FloatingPointTypes.contains(type.mName))
    return "number";
  if (type.mName == "std::string")
    return "string";
  if (type.mName == "std::pair") {
    return std::format("{{{}, {}}}", ToLuaType(type.mTemplateParameters.front()),
                       ToLuaType(type.mTemplateParameters.back()));
  }
  if (type.mName == "std::tuple") {
    bool isFirst = true;
    std::stringstream ss;
    ss << "{";
    for (auto &t: type.mTemplateParameters) {
      if (isFirst)
        isFirst = false;
      else
        ss << ", ";
      ss << ToLuaType(t);
    }
    ss << "}";
    return ss.str();
  }
  if (auto cls = mTranslatedProject.GetClass(type.mName)) {
    if (cls->mEnum)
      return cls->mName;
    else
      return cls->mName + St::LuaMetatableSuffix;
  }
  if (ti.CppKeyedContainers.contains(type.mName)) {
    return std::format("table<{}, {}>", ToLuaType(type.mTemplateParameters.front()),
                       ToLuaType(type.mTemplateParameters.back()));
  }
  if (ti.CppContainers.contains(type.mName)) {
    return std::format("{}[]", ToLuaType(type.mTemplateParameters.front()));
  }
  return "any";
}
} // namespace holgen
