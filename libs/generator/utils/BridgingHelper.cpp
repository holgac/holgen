#include "BridgingHelper.h"
#include "core/Exception.h"
#include "core/St.h"

namespace holgen {
Type BridgingHelper::ConvertType(const TranslatedProject &project, const Type &type,
                                 bool isReturnType, const DefinitionSource &definitionSource) {

  if (TypeInfo::Get().CppPrimitives.contains(type.mName) || type.mName == "void") {
    return type;
  }
  if (type.mName == "std::string") {
    return Type{"char", PassByType::Pointer, Constness::Const};
  }
  if (type.mName == "std::vector" || type.mName == "std::span" || type.mName == "std::array") {
    auto underlying = ConvertType(project, type.mTemplateParameters.front(), isReturnType, definitionSource);
    if (underlying.mType == PassByType::Pointer) {
      ++underlying.mPointerDepth;
    } else {
      underlying.mType = PassByType::Pointer;
    }
    return underlying;
  }
  if (auto cls = project.GetClass(type.mName)) {
    auto res = type;
    THROW_IF(res.mType == PassByType::Value && cls->IsProxyable(),
             "Proxy class {} is returned from value in {}", cls->mName, definitionSource)
    if (res.mType == PassByType::Reference) {
      res.mType = PassByType::Pointer;
    }
    if (!isReturnType && res.mType == PassByType::Value) {
      res.mType = PassByType::Pointer;
    }
    return res;
  }
  THROW("Don't know how to represent {} ({}) objects in c wrappers!", type.ToString(false, true), definitionSource);
}

MethodArgument& BridgingHelper::AddArgument(const TranslatedProject &project, MethodBase &method,
                                 const MethodArgument &arg,
                                 const DefinitionSource &definitionSource) {
  auto& addedArg = method.mArguments.emplace_back(arg.mName,
                                 ConvertType(project, arg.mType, false, definitionSource));
  AddAuxiliaryArguments(project, method, arg.mType, arg.mName, false);
  return addedArg;
}

void BridgingHelper::AddAuxiliaryArguments(const TranslatedProject &project, MethodBase &method,
                                          const Type &type, const std::string &argPrefix, bool isReturnType) {
  (void)project;
  if (type.mName == "std::vector" || type.mName == "std::span") {
    auto& arg = method.mArguments.emplace_back(argPrefix + St::CSharpAuxiliarySizeSuffix, Type{"size_t"});
    if (isReturnType)
      arg.mType.mType = PassByType::Pointer;
  }
}
} // namespace holgen
