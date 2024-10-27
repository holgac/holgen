#include "CSharpHelper.h"

namespace holgen {
std::string CSharpHelper::TypeRepresentation(const Type &other, const TranslatedProject &project) {
  (void)project;
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    // this ignores const qualifiers
    return it->second;
  }
  return other.mName;
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
