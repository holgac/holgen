#pragma once

#include <string>
#include <vector>
#include <format>
#include "core/Iterators.h"


namespace holgen {
  struct DefinitionSource {
    std::string mSource;
    size_t mLine = -1;
    size_t mColumn = -1;
  };

  struct TypeDefinition {
    std::string mName;
    std::vector<TypeDefinition> mTemplateParameters;
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

  struct FieldDefinition {
    TypeDefinition mType;
    std::string mName;
    std::vector<AnnotationDefinition> mAnnotations;
    std::string mDefaultValue;
    DefinitionSource mDefinitionSource;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;

    auto GetAnnotations(const std::string &name) const {
      return NameFilterForEachWrapper(name, mAnnotations);
    }
  };

  struct FunctionArgumentDefinition {
    TypeDefinition mType;
    std::string mName;
    bool mIsOut = false;
    DefinitionSource mDefinitionSource;
  };

  struct FunctionDefinition {
    std::string mName;
    TypeDefinition mReturnType;
    std::vector<FunctionArgumentDefinition> mArguments;
    std::vector<AnnotationDefinition> mAnnotations;
    DefinitionSource mDefinitionSource;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<AnnotationDefinition> mAnnotations;
    std::vector<FunctionDefinition> mFunctions;
    DefinitionSource mDefinitionSource;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
    [[nodiscard]] const FieldDefinition *GetField(const std::string &name) const;
    [[nodiscard]] const FunctionDefinition *GetFunction(const std::string &name) const;
    [[nodiscard]] const FieldDefinition *GetIdField() const;

    auto GetAnnotations(const std::string &name) {
      return NameFilterForEachWrapper(name, mAnnotations);
    }
  };

  struct EnumEntryDefinition {
    std::string mName;
    std::string mValue;
    std::vector<AnnotationDefinition> mAnnotations;
    DefinitionSource mDefinitionSource;
  };

  struct EnumDefinition {
    std::string mName;
    std::string mInvalidValue;
    std::vector<EnumEntryDefinition> mEntries;
    std::vector<AnnotationDefinition> mAnnotations;
    DefinitionSource mDefinitionSource;
    [[nodiscard]] const EnumEntryDefinition *GetEnumEntry(const std::string &name) const;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
  };

  struct ProjectDefinition {
    std::vector<StructDefinition> mStructs;
    std::vector<EnumDefinition> mEnums;
    [[nodiscard]] const StructDefinition *GetStruct(const std::string &name) const;
    [[nodiscard]] const EnumDefinition *GetEnum(const std::string &name) const;
  };


}

template<>
struct std::formatter<holgen::DefinitionSource> : std::formatter<std::string> {
  auto format(const holgen::DefinitionSource &definitionSource, format_context &ctx) const {
    if (definitionSource.mLine == (decltype(definitionSource.mLine)) (-1) ||
        definitionSource.mColumn == (decltype(definitionSource.mColumn)) (-1))
      return std::format_to(ctx.out(), "{}", definitionSource.mSource);
    return std::format_to(ctx.out(), "{}:{}:{}", definitionSource.mSource,
                          definitionSource.mLine + 1, definitionSource.mColumn + 1);
  }
};

