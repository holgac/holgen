#include "BridgingHelper.h"
#include "core/Exception.h"
#include "generator/TranslatedProject.h"

namespace holgen {
Type BridgingHelper::ConvertType(const TranslatedProject &project, const Type &type,
                                 bool isReturnType, const DefinitionSource &definitionSource) {

  if (TypeInfo::Get().CppPrimitives.contains(type.mName) || type.mName == "void") {
    return type;
  }
  if (type.mName == "std::string") {
    return Type{"char", PassByType::Pointer, Constness::Const};
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
  THROW("Don't know how to represent {} objects in c wrappers!", type.ToString(false, true));
}
} // namespace holgen
