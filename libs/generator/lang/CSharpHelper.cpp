#include "CSharpHelper.h"

#include "generator/TranslatedProject.h"

#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
std::string CSharpHelper::RepresentationInNative(const Type &other,
                                                 const TranslatedProject &project,
                                                 bool prependRef) {
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

std::string CSharpHelper::RepresentationInManaged(const Type &other,
                                                  const TranslatedProject &project) {
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

std::string CSharpHelper::Representation(const Type &other, const TranslatedProject &project,
                                         InteropType interopType, bool prependRef) {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
    return RepresentationInNative(other, project, prependRef);
  case InteropType::NativeToManaged:
    return RepresentationInManaged(other, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
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

std::string CSharpHelper::VariableRepresentation(const Type &other, const std::string &variableName,
                                                 const TranslatedProject &project,
                                                 InteropType interopType, bool prependRef) {
  switch (interopType) {
  case InteropType::Internal:
    return variableName;
  case InteropType::ManagedToNative:
    return VariableRepresentationInNative(other, variableName, project, prependRef);
  case InteropType::NativeToManaged:
    return VariableRepresentationInManaged(other, variableName, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::VariableRepresentationInNative(const Type &other,
                                                         const std::string &variableName,
                                                         const TranslatedProject &project,
                                                         bool prependRef) {
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    return variableName;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->IsProxyable()) {
      return std::format("{}.{}", variableName, St::CSharpProxyObjectPointerFieldName);
    } else {
      return std::format("{}{}.{}", prependRef ? "ref " : "", variableName,
                         St::CSharpMirroredStructFieldName);
    }
  }
  return variableName;
}

std::string CSharpHelper::VariableRepresentationInManaged(const Type &other,
                                                          const std::string &variableName,
                                                          const TranslatedProject &project) {
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    return variableName;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->mStruct &&
        cls->mStruct->GetMatchingAttribute(Annotations::Script, Annotations::Script_AlwaysMirror)) {
      return std::format("new {}({})", other.mName, variableName);
    }
  }
  return variableName;
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
      {"int8_t", "sbyte"},      {"int16_t", "short"},       {"int32_t", "int"},
      {"int64_t", "long"},      {"std::ptrdiff_t", "long"}, {"uint8_t", "byte"},
      {"uint16_t", "ushort"},   {"uint32_t", "uint"},       {"uint64_t", "ulong"},
      {"size_t", "ulong"},      {"float", "float"},         {"double", "double"},
      {"std::string", "string"}};
}
} // namespace holgen
