#pragma once

#include <string>
#include <vector>
#include "core/Iterators.h"


namespace holgen {
  struct TypeDefinition {
    std::string mName;
    std::vector<TypeDefinition> mTemplateParameters;
    bool operator==(const TypeDefinition &rhs) const;
  };

  // @annotation(attribute=5)
  struct AnnotationAttributeDefinition {
    std::string mName;
    TypeDefinition mValue;
  };

  struct AnnotationDefinition {
    std::string mName;
    std::vector<AnnotationAttributeDefinition> mAttributes;
    [[nodiscard]] const AnnotationAttributeDefinition *GetAttribute(const std::string &name) const;
  };

  struct FieldDefinition {
    TypeDefinition mType;
    std::string mName;
    std::vector<AnnotationDefinition> mAnnotations;
    std::string mDefaultValue;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;

    auto GetAnnotations(const std::string &name) const {
      return NameFilterForEachWrapper(name, mAnnotations);
    }
  };

  struct FunctionArgumentDefinition {
    TypeDefinition mType;
    std::string mName;
    bool mIsOut = false;
  };

  struct FunctionDefinition {
    std::string mName;
    TypeDefinition mReturnType;
    std::vector<FunctionArgumentDefinition> mArguments;
    std::vector<AnnotationDefinition> mAnnotations;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<AnnotationDefinition> mAnnotations;
    std::vector<FunctionDefinition> mFunctions;
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
  };

  struct EnumDefinition {
    std::string mName;
    std::string mInvalidValue;
    std::vector<EnumEntryDefinition> mEntries;
    std::vector<AnnotationDefinition> mAnnotations;
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