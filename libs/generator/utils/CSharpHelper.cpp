#include "CSharpHelper.h"

#include "generator/TranslatedProject.h"

#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
bool CSharpHelper::NeedsDeleterArgument(const Type &type) const {
  return CppTypesConvertibleToCSharpArray.contains(type.mName);
}

bool CSharpHelper::NeedsSizeArgument(const Type &type) const {
  return type.mName == "std::vector" || type.mName == "std::span";
}

CSharpType CSharpHelper::ConvertFieldType(const Type &type, const TranslatedProject &project,
                                          InteropType interopType, bool returnType) const {
  (void)interopType;
  CSharpType out;
  auto cls = project.GetClass(type.mName);
  auto it = CppTypeToCSharpType.find(type.mName);
  if (type.mName == "char" && type.mType == PassByType::Pointer) {
    out.mName = "string";
    out.mArrayDepth = type.mPointerDepth;
    THROW_IF(out.mArrayDepth > 1, "{} cannot be represented in c#: pointer level too deep!",
             type.ToString(true));
  } else if (it != CppTypeToCSharpType.end()) {
    out.mName = it->second;
  } else if (cls) {
    auto csCls = project.GetCSharpClass(type.mName);
    if (interopType != InteropType::Internal ||
        (csCls && csCls->mStaticness == Staticness::Static)) {
      if (cls->IsProxyable()) {
        out.mName = "IntPtr";
      } else {
        out.mName = std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName);
      }
    } else {
      out.mName = cls->mName;
    }
  } else if (type.mName == "std::span" || type.mName == "std::array" ||
             type.mName == "std::vector") {
    if (interopType != InteropType::Internal && returnType) {
      out.mName = "IntPtr";
    } else {
      out = ConvertFieldType(type.mTemplateParameters.front(), project, interopType, false);
      ++out.mArrayDepth;
    }
  } else {
    THROW("Unexpected type: {}", type.mName);
  }
  return out;
}

CSharpHelper &CSharpHelper::Get() {
  static CSharpHelper instance;
  return instance;
}

CSharpHelper::CSharpHelper() {
  CppTypeToCSharpType = {
      {"int8_t", "sbyte"},  {"int16_t", "short"},       {"int32_t", "int"},  {"int", "int"},
      {"int64_t", "long"},  {"std::ptrdiff_t", "long"}, {"uint8_t", "byte"}, {"uint16_t", "ushort"},
      {"uint32_t", "uint"}, {"uint64_t", "ulong"},      {"size_t", "ulong"}, {"float", "float"},
      {"double", "double"}, {"std::string", "string"},  {"void", "void"},    {"bool", "bool"}};
  CppTypesConvertibleToCSharpArray = {"std::vector", "std::array", "std::span"};
  CSharpTypesSupportedByMarshalCopy = {"byte", "short", "int", "long", "float", "double"};
}

} // namespace holgen
