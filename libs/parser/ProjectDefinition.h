#pragma once

#include <format>
#include <string>
#include <vector>
#include <optional>
#include "core/Iterators.h"

namespace holgen {
struct ProjectDefinition;

enum class Constness {
  Const,
  NotConst,
};

enum class PassByType {
  Value,
  Reference,
  Pointer,
  MoveReference,
};

struct DefinitionSource {
  std::string mSource;
  size_t mLine = -1;
  size_t mColumn = -1;
};

enum class TypeDefinitionType {
  Value,
  Pointer,
  Reference,
};

struct TypeDefinition {
  std::string mName;
  std::vector<TypeDefinition> mTemplateParameters;
  // positive values indicate array; 0 means a regular value
  std::string mArraySize;
  TypeDefinitionType mType;
  bool operator==(const TypeDefinition &rhs) const;
};

// @annotation(attribute=5)
struct AnnotationAttributeDefinition {
  std::string mName;
  TypeDefinition mValue;
  DefinitionSource mDefinitionSource;
};

struct AnnotationDefinition {
  std::string mName;
  std::vector<AnnotationAttributeDefinition> mAttributes;
  DefinitionSource mDefinitionSource;
  [[nodiscard]] const AnnotationAttributeDefinition *GetAttribute(const std::string &name) const;

  auto GetAttributes(const std::string &name) const {
    return NameFilterForEachWrapper(name, mAttributes);
  }
};

struct AnnotationsMixin {
  std::vector<AnnotationDefinition> mAnnotations;
  [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;

  auto GetAnnotations(const std::string &name) const {
    return NameFilterForEachWrapper(name, mAnnotations);
  }

  [[nodiscard]] const AnnotationAttributeDefinition *
      GetMatchingAttribute(const std::string &annotationName, const std::string &attributeName,
                           const std::optional<std::string> &attributeValue = std::nullopt) const;
  [[nodiscard]] const AnnotationDefinition *
      GetMatchingAnnotation(const std::string &annotationName, const std::string &attributeName,
                            const std::optional<std::string> &attributeValue = std::nullopt) const;
};

struct FieldDefinition : public AnnotationsMixin {
  TypeDefinition mType;
  std::string mName;
  std::optional<std::string> mDefaultValue = std::nullopt;
  DefinitionSource mDefinitionSource;
};

enum class Nullability {
  Nullable,
  NotNullable,
};

enum class Newness {
  New,
  NotNew,
};

struct FunctionArgumentDefinition {
  TypeDefinition mType;
  std::string mName;
  Constness mConstness = Constness::Const;
  Nullability mNullability = Nullability::NotNullable;
  Newness mNewness = Newness::NotNew;
  std::optional<std::string> mDefaultValue = std::nullopt;
  DefinitionSource mDefinitionSource;
};

enum class FunctionReturnTypeCategory {
  NewObject,
  Reference,
  Pointer,
};

struct FunctionReturnTypeDefinition {
  TypeDefinition mType;
  Constness mConstness = Constness::NotConst;
  FunctionReturnTypeCategory mCategory = FunctionReturnTypeCategory::NewObject;
};

struct FunctionDefinition : public AnnotationsMixin {
  std::string mName;
  FunctionReturnTypeDefinition mReturnType;
  std::vector<FunctionArgumentDefinition> mArguments;
  DefinitionSource mDefinitionSource;
};

struct StructDefinition : public AnnotationsMixin {
  std::string mName;
  std::vector<FieldDefinition> mFields;
  std::vector<FunctionDefinition> mFunctions;
  std::vector<std::string> mMixins;
  bool mIsMixin = false;
  DefinitionSource mDefinitionSource;
  [[nodiscard]] const FieldDefinition *GetField(const std::string &name) const;
  [[nodiscard]] const FunctionDefinition *GetFunction(const std::string &name) const;
  [[nodiscard]] const FieldDefinition *GetIdField() const;
  [[nodiscard]] const FunctionDefinition *GetHashFunction(const ProjectDefinition &project) const;
  [[nodiscard]] const FunctionDefinition *GetToStringFunction(const ProjectDefinition &project) const;
};

struct EnumEntryDefinition : public AnnotationsMixin {
  std::string mName;
  std::string mValue;
  DefinitionSource mDefinitionSource;
};

enum class EnumDefinitionType {
  Enum,
  Bitmap,
};

struct EnumDefinition : public AnnotationsMixin {
  std::string mName;
  std::string mInvalidValue;
  std::vector<EnumEntryDefinition> mEntries;
  DefinitionSource mDefinitionSource;
  EnumDefinitionType mType;
  [[nodiscard]] const EnumEntryDefinition *GetEnumEntry(const std::string &name) const;
  [[nodiscard]] const EnumEntryDefinition *GetDefaultEntry() const;
};

struct ProjectDefinition {
  std::vector<StructDefinition> mStructs;
  std::vector<StructDefinition> mMixins;
  std::vector<EnumDefinition> mEnums;
  [[nodiscard]] const StructDefinition *GetStruct(const std::string &name) const;
  [[nodiscard]] const EnumDefinition *GetEnum(const std::string &name) const;
};


} // namespace holgen

template <>
struct std::formatter<holgen::DefinitionSource> : std::formatter<std::string> {
  auto format(const holgen::DefinitionSource &definitionSource, format_context &ctx) const {
    if (definitionSource.mLine == (decltype(definitionSource.mLine))(-1) ||
        definitionSource.mColumn == (decltype(definitionSource.mColumn))(-1))
      return std::format_to(ctx.out(), "{}", definitionSource.mSource);
    return std::format_to(ctx.out(), "{}:{}:{}", definitionSource.mSource,
                          definitionSource.mLine + 1, definitionSource.mColumn + 1);
  }
};
