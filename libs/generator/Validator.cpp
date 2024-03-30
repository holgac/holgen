#include "Validator.h"
#include <set>
#include <string>
#include "TypeInfo.h"
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

    std::string ToString(const Class &cls, const ClassMethod &method) {
      if (method.mFunction)
        return std::format("{}.{} ({})", cls.mName, method.mFunction->mName, method.mFunction->mDefinitionSource);
      else
        return std::format("{}.{}", cls.mName, method.mName);
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
    ValidateType(field.mType, cls, false, ToString(cls, field));
  }

  void Validator::ValidateType(const Type &type, const Class &cls, bool acceptVoid, const std::string &source) const {
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
      ValidateType(type.mTemplateParameters[0], cls, false, source);
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
      ValidateType(type.mTemplateParameters[0], cls, false, source);
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
      ValidateType(type.mTemplateParameters[1], cls, false, source);
    } else if (type.mName == "void") {
      THROW_IF(!acceptVoid, "Invalid void usage in {}", source);
      THROW_IF(type.mType == PassByType::Reference || type.mType == PassByType::MoveReference,
               "Void cannot be passed by reference in {}", source);
      THROW_IF(type.mTemplateParameters.size() > 0, "Void cannot have template parameters in {}", source);
    } else if (auto cls2 = mProject.GetClass(type.mName)) {
      THROW_IF(!cls2->mStruct && !cls2->mEnum, "Internal type {} used by {}", type.mName, source);
    } else if (auto usingStatement = cls.GetUsing(type.mName)) {
      ValidateType(usingStatement->mSourceType, cls, acceptVoid, source);
    } else if (cls.GetForwardDeclaration(type.mName)) {
      THROW_IF(type.mType == PassByType::Value,
               "Forward declared type {} used by {} cannot be passed by value",
               type.mName, source);
    } else {
      THROW("Unknown type {} used by {}", type.mName, source);
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

  void Validator::NewMethod(const Class &cls, const ClassMethod &method) const {
    for (auto &method2: cls.GetMethods(method.mName)) {
      if (method.mConstness != method2.mConstness)
        continue;
      if (method.mArguments.size() != method2.mArguments.size())
        continue;
      for (size_t i = 0; i < method.mArguments.size(); ++i) {
        auto &arg = method.mArguments[i];
        auto &arg2 = method2.mArguments[i];
        if (arg.mType.mName != arg2.mType.mName)
          continue;
        if (arg.mType.mType != arg2.mType.mType) {
          if (arg.mType.mType == PassByType::Pointer || arg2.mType.mType == PassByType::Pointer ||
              arg.mType.mType == PassByType::MoveReference || arg2.mType.mType == PassByType::MoveReference)
            continue;
        }
        if (arg.mType.mTemplateParameters.size() != arg2.mType.mTemplateParameters.size())
          continue;
        for (size_t j = 0; j < arg.mType.mTemplateParameters.size(); ++j) {
          if (arg.mType.mTemplateParameters[j].mName != arg2.mType.mTemplateParameters[j].mName)
            continue;
        }
        if (arg.mType.mFunctionalTemplateParameters.size() != arg2.mType.mFunctionalTemplateParameters.size())
          continue;
        for (size_t j = 0; j < arg.mType.mFunctionalTemplateParameters.size(); ++j) {
          if (arg.mType.mFunctionalTemplateParameters[j].mName != arg2.mType.mFunctionalTemplateParameters[j].mName)
            continue;
        }
        THROW("Duplicate method: {} and {}", ToString(cls, method2), ToString(cls, method));
      }
    }
    ValidateType(method.mReturnType, cls, true, ToString(cls, method));
    for (auto &arg: method.mArguments) {
      ValidateType(arg.mType, cls, false, ToString(cls, method));
    }
  }
}
