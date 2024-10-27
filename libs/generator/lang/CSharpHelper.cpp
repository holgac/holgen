#include "CSharpHelper.h"

#include "generator/TranslatedProject.h"

#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
std::string CSharpHelper::RepresentationInNative(const Type &other,
                                                 const TranslatedProject &project) {
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    // this ignores const qualifiers
    return it->second;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->mStruct &&
        cls->mStruct->GetMatchingAttribute(Annotations::Script, Annotations::Script_AlwaysMirror)) {
      return std::format("{}.{}", other.mName, St::CSharpMirroredStructStructName);
    }
  }
  return other.mName;
}

std::string CSharpHelper::RepresentationInManaged(const Type &other,
                                                  const TranslatedProject &project) {
  (void)project;
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    // this ignores const qualifiers
    return it->second;
  }
  return other.mName;
}

std::string CSharpHelper::Representation(const Type &other, const TranslatedProject &project,
                                         InteropType interopType) {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
    return RepresentationInNative(other, project);
  case InteropType::NativeToManaged:
    return RepresentationInManaged(other, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::VariableRepresentation(const Type &other, const std::string &variableName,
                                                 const TranslatedProject &project,
                                                 InteropType interopType) {
  switch (interopType) {
  case InteropType::Internal:
    return variableName;
  case InteropType::ManagedToNative:
    return ManagedVariableToNative(other, variableName, project);
  case InteropType::NativeToManaged:
    return NativeVariableToManaged(other, variableName, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::ManagedVariableToNative(const Type &other,
                                                  const std::string &variableName,
                                                  const TranslatedProject &project) {
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    return variableName;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->mStruct &&
        cls->mStruct->GetMatchingAttribute(Annotations::Script, Annotations::Script_AlwaysMirror)) {
      return std::format("{}.{}", variableName, St::CSharpMirroredStructFieldName);
    }
  }
  return variableName;
}

std::string CSharpHelper::NativeVariableToManaged(const Type &other,
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

CSharpHelper &CSharpHelper::Get() {
  static CSharpHelper instance;
  return instance;
}

CSharpHelper::CSharpHelper() {
  CppTypeToCSharpType = {
      {"int8_t", "sbyte"}, {"int16_t", "short"},   {"int32_t", "int"},   {"int64_t", "long"},
      {"uint8_t", "byte"}, {"uint16_t", "ushort"}, {"uint32_t", "uint"}, {"uint64_t", "ulong"},
      {"float", "float"},  {"double", "double"},
  };
}
} // namespace holgen
