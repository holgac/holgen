#include "LuaGeneratorBase.h"
#include "core/Annotations.h"

namespace holgen {
bool LuaGeneratorBase::ShouldProcess(const Class &cls) const {
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

bool LuaGeneratorBase::ShouldProcess(const ClassMethod &method) const {
  return method.mExposeToLua;
}

std::pair<std::string, CodeBlock>
    LuaGeneratorBase::ConstructMethodArgumentsAndAnnotations(const ClassMethod &method) const {
  std::pair<std::string, CodeBlock> retVal;
  std::stringstream argsStr;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (arg.mType.mName == "lua_State") {
      THROW_IF(!isFirst, "lua_State argument in unexpected position!")
      continue;
    }
    retVal.second.Add("---@param {} {}", arg.mName, ToLuaType(arg.mType));
    if (isFirst)
      isFirst = false;
    else
      argsStr << ", ";
    argsStr << arg.mName;
  }
  retVal.first = argsStr.str();
  return retVal;
}

std::string LuaGeneratorBase::ToLuaType(const Type &type) const {
  auto &ti = TypeInfo::Get();
  if (type.mName == "void" && type.mType == PassByType::Pointer)
    return "userdata";
  if (type.mName == "bool")
    return "boolean";
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
      return mNamingConvention.LuaMetatableName(*cls);
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
