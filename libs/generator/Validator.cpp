#include "Validator.h"
#include <set>
#include <string>
#include "core/Annotations.h"
#include "TypeInfo.h"
#include "core/St.h"
#include "core/Exception.h"
#include "TranslatedProject.h"

namespace holgen {
  namespace {
    std::set<std::string> ReservedKeywords{
        "struct",
        "class",
        "namespace",
        "template",
    };
    std::set<std::string> CustomTypes{
        "Ref",
    };

    std::string ToString(const Class &cls) {
      if (cls.mStruct)
        return std::format("{} ({})", cls.mStruct->mName, cls.mStruct->mDefinitionSource);
      else if (cls.mEnum)
        return std::format("{} ({})", cls.mEnum->mName, cls.mEnum->mDefinitionSource);
      else
        return std::format("{}", cls.mName);
    }

    std::string ToString(const Class &cls, const FieldDefinition &fieldDefinition) {
      return std::format("{}.{} ({})", cls.mName, fieldDefinition.mName, fieldDefinition.mDefinitionSource);
    }

    std::string ToString(const Class &cls, const ClassField &field) {
      if (field.mField)
        return ToString(cls, *field.mField);
      else
        return std::format("{}.{}", cls.mName, field.mName);
    }
  }

  Validator::Validator(TranslatedProject &project) : mProject(project) {}

  void Validator::NewClass(const Class &cls) const {
    THROW_IF(ReservedKeywords.contains(cls.mName), "{} is a reserved keyword", ToString(cls));
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(cls.mName), "{} is a reserved keyword", ToString(cls));
    auto dup = mProject.GetClass(cls.mName);
    THROW_IF(dup, "Duplicate class: {} and {}", ToString(*dup), ToString(cls));
  }

  void Validator::NewField(const Class &cls, const ClassField &field) const {
    THROW_IF(ReservedKeywords.contains(field.mName), "{} is a reserved keyword", ToString(cls, field));
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(field.mName), "{} is a reserved keyword", ToString(cls, field));
    auto dup = cls.GetField(field.mName);
    THROW_IF(dup, "Duplicate field: {} and {}", ToString(cls, *dup), ToString(cls, field));
    ValidateType(field.mType, ToString(cls, field));
  }

  void Validator::ValidateType(const Type &type, const std::string &source) const {
    if (TypeInfo::Get().CppBasicTypes.contains(type.mName)) {
      THROW_IF(type.mTemplateParameters.size() > 0,
               "Primitive type {} used by {} cannot have template parameters",
               type.mName, source);
      THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
               "Primitive type {} used by {} cannot have functional template parameters",
               type.mName, source);
    } else if (TypeInfo::Get().CppIndexedContainers.contains(type.mName)) {
      THROW_IF(type.mTemplateParameters.size() != 1,
               "Container type {} used by {} should have a single template parameter",
               type.mName, source);
      THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
               "Container type {} used by {} cannot have functional template arguments",
               type.mName, source);
      ValidateType(type.mTemplateParameters[0], source);
    } else if (TypeInfo::Get().CppSets.contains(type.mName)) {
      THROW_IF(type.mTemplateParameters.size() != 1,
               "Set type {} used by {} should have a single template parameter",
               type.mName, source);
      THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
               "Set type {} used by {} cannot have functional template arguments",
               type.mName, source);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mTemplateParameters[0].mName),
               "Set type {} used by {} should have a keyable template parameter, found {}",
               type.mName, source, type.mTemplateParameters[0].mName);
      ValidateType(type.mTemplateParameters[0], source);
    } else if (TypeInfo::Get().CppKeyedContainers.contains(type.mName)) {
      THROW_IF(type.mTemplateParameters.size() != 2,
               "Map type {} used by {} should have two template parameters",
               type.mName, source);
      THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
               "Map type {} used by {} cannot have functional template arguments",
               type.mName, source);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mTemplateParameters[0].mName),
               "Map type {} used by {} should have a keyable first template parameter, found {}",
               type.mName, source, type.mTemplateParameters[0].mName);
      ValidateType(type.mTemplateParameters[1], source);
    } else {
      auto cls = mProject.GetClass(type.mName);
      THROW_IF(!cls, "Unknown type {} used by {}", type.mName, source);
      THROW_IF(!cls->mStruct && !cls->mEnum, "Internal type {} used by {}", type.mName, source);
    }
  }

  void Validator::RefField(const Class &cls, const FieldDefinition &fieldDefinition) const {
    THROW_IF(fieldDefinition.mType.mTemplateParameters.size() != 1,
             "Ref field {} should have a single template parameter", ToString(cls, fieldDefinition));
    auto &underlyingName = fieldDefinition.mType.mTemplateParameters[0].mName;
    auto referencedType = mProject.GetClass(underlyingName);
    THROW_IF(!referencedType || !referencedType->mStruct,
             "Ref field {} references {} which is not a struct defined in this project",
             ToString(cls, fieldDefinition), underlyingName);
  }
}
