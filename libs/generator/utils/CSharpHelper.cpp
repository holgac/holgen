#include "CSharpHelper.h"

#include "generator/TranslatedProject.h"

#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
bool CSharpHelper::NeedsDeleter(const Type &type) {
  return CppTypesConvertibleToCSharpArray.contains(type.mName);
}

bool CSharpHelper::NeedsSizeArgument(const Type &type) {
  return type.mName == "std::vector" || type.mName == "std::span";
}

CSharpType CSharpHelper::ConvertType(const Type &type, const TranslatedProject &project,
                                     InteropType interopType, bool returnType) {
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
    if (interopType == InteropType::NativeToManaged || (csCls && csCls->mStaticness == Staticness::Static)) {
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
    if (interopType == InteropType::NativeToManaged && returnType) {
      out.mName = "IntPtr";
    } else {
      out = ConvertType(type.mTemplateParameters.front(), project, interopType, false);
      ++out.mArrayDepth;
    }
  } else {
    THROW("Unexpected type: {}", type.mName);
  }
  return out;
}

void CSharpHelper::AddAttributes(std::list<std::string> &attributes, const Type &type,
                                 const CSharpType &csType, InteropType interopType,
                                 bool isReturnType, size_t sizeArgIndex) {
  (void)interopType;
  std::string prefix = isReturnType ? "return: " : "";
  if (type.mName == "char" && type.mType == PassByType::Pointer) {
    if (csType.mArrayDepth == 0) {
      attributes.emplace_back(std::format("{}MarshalAs(UnmanagedType.LPStr)", prefix));
    } else {
      attributes.emplace_back(std::format(
          "{}MarshalAs(UnmanagedType.LPArray, ArraySubType=UnmanagedType.LPStr, SizeParamIndex={})",
          prefix, sizeArgIndex));
    }
  }
  if (type.mName == "std::array") {
    attributes.emplace_back(std::format("{}MarshalAs(UnmanagedType.LPArray, SizeConst={})", prefix,
                                        type.mTemplateParameters.back().mName));
  } else if (type.mName == "std::vector" || type.mName == "std::span") {
    attributes.emplace_back(
        std::format("{}MarshalAs(UnmanagedType.LPArray, SizeParamIndex={})", prefix, sizeArgIndex));
  }
}

void CSharpHelper::AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments, const Type &type,
                                     const std::string &argPrefix, InteropType interopType,
                                     bool isReturnValue) {
  (void)interopType;
  if (type.mName == "std::vector" || type.mName == "std::span") {
    auto &arg = arguments.emplace_back(
        std::format("{}{}", argPrefix, St::CSharpAuxiliarySizeSuffix), CSharpType{"ulong"});
    if (isReturnValue) {
      arg.mType.mType = CSharpPassByType::Out;
    }
  }
  if (isReturnValue &&
      (type.mName == "std::array" || type.mName == "std::vector" || type.mName == "std::span")) {
    auto &deleterArg =
        arguments.emplace_back(St::DeferredDeleterArgumentName, CSharpType{"IntPtr"});
    deleterArg.mType.mType = CSharpPassByType::Out;
  }
}

std::string CSharpHelper::StringifyPassedExtraArguments(const Type &type,
                                                        const std::string &argPrefix,
                                                        InteropType interopType) {
  (void)interopType;
  if (type.mName == "std::vector" || type.mName == "std::span") {
    return std::format(", (ulong){}.Length", argPrefix);
  }
  return "";
}

std::string CSharpHelper::RepresentationInNative(const Type &other, const Type &originalType,
                                                 const TranslatedProject &project,
                                                 bool prependRef) {
  (void)originalType;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    switch (other.mPointerDepth) {
    case 0:
      return "string";
    case 1:
      return "string[]";
    default:
      THROW("Unexpected pointer-to-pointer-to-pointer in {}", other.ToString(true, false));
    }
  }

  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    if (other.mType == PassByType::Pointer)
      return std::format("{}[]", it->second);
    return it->second;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->IsProxyable()) {
      return "IntPtr";
    } else {
      return std::format("{}{}.{}", prependRef ? "ref " : "", other.mName,
                         St::CSharpMirroredStructStructName);
    }
  }
  return other.mName;
}

std::string CSharpHelper::RepresentationInManaged(const Type &other, const Type &originalType,
                                                  const TranslatedProject &project) {
  (void)originalType;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    switch (other.mPointerDepth) {
    case 0:
      return "string";
    case 1:
      return "string[]";
    default:
      THROW("Unexpected pointer-to-pointer-to-pointer in {}", other.ToString(true, false));
    }
  }

  (void)project;
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    if (other.mType == PassByType::Pointer)
      return std::format("{}[]", it->second);
    return it->second;
  }
  return other.mName;
}

std::string CSharpHelper::MarshallingInfo(const Type &other, const TranslatedProject &project,
                                          InteropType interopType) {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
  case InteropType::NativeToManaged:
    return MarshallingInfo(other, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                               InteropType interopType, size_t sizeArgumentIdx) {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
  case InteropType::NativeToManaged:
    return ArrayMarshallingInfo(other, project, sizeArgumentIdx);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::VariableRepresentation(const CSharpType &type,
                                                 const std::string &variableName,
                                                 const TranslatedProject &project,
                                                 InteropType interopType) {
  switch (interopType) {
  case InteropType::Internal:
    return variableName;
  case InteropType::ManagedToNative:
    return VariableRepresentationInNative(type, variableName, project);
  case InteropType::NativeToManaged:
    return VariableRepresentationInManaged(type, variableName, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::VariableRepresentationInNative(const CSharpType &type,
                                                         const std::string &variableName,
                                                         const TranslatedProject &project) {
  if (auto cls = project.GetClass(type.mName)) {
    if (cls->IsProxyable()) {
      return std::format("{}{}.{}", type.mType, variableName,
                         St::CSharpProxyObjectPointerFieldName);
    } else {
      return std::format("{}{}.{}", type.mType, variableName, St::CSharpMirroredStructFieldName);
    }
  }
  return std::format("{}{}", type.mType, variableName);
}

std::string CSharpHelper::VariableRepresentationInManaged(const CSharpType &type,
                                                          const std::string &variableName,
                                                          const TranslatedProject &project) {

  if (auto cls = project.GetClass(type.mName)) {
    if (!cls->IsProxyable())
      return std::format("new {}({})", type.mName, variableName);
  }
  return std::format("{}{}", type.mType, variableName);
}

std::string CSharpHelper::MarshallingInfo(const Type &other, const TranslatedProject &project) {
  (void)project;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    return "[MarshalAs(UnmanagedType.LPStr)] ";
  }
  return "";
}

std::string CSharpHelper::ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                               size_t sizeArgumentIdx) {
  (void)project;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    return std::format(
        "[MarshalAs(UnmanagedType.LPArray, SizeParamIndex={}, ArraySubType=UnmanagedType.LPStr)] ",
        sizeArgumentIdx);
  }
  return std::format("[MarshalAs(UnmanagedType.LPArray, SizeParamIndex={})] ", sizeArgumentIdx);
}

CSharpHelper &CSharpHelper::Get() {
  static CSharpHelper instance;
  return instance;
}

CSharpHelper::CSharpHelper() {
  CppTypeToCSharpType = {
      {"int8_t", "sbyte"},       {"int16_t", "short"},       {"int32_t", "int"},
      {"int64_t", "long"},       {"std::ptrdiff_t", "long"}, {"uint8_t", "byte"},
      {"uint16_t", "ushort"},    {"uint32_t", "uint"},       {"uint64_t", "ulong"},
      {"size_t", "ulong"},       {"float", "float"},         {"double", "double"},
      {"std::string", "string"}, {"void", "void"},           {"bool", "bool"}};
  CppTypesConvertibleToCSharpArray = {"std::vector", "std::array", "std::span"};
  CSharpTypesSupportedByMarshalCopy = {"byte", "short", "int", "long", "float", "double"};
}
} // namespace holgen
