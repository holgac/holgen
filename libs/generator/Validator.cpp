#include "Validator.h"
#include <set>
#include <string>
#include "core/Annotations.h"
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

    std::string ToString(const AnnotationDefinition &annotation) {
      return std::format("{} ({})", annotation.mName, annotation.mDefinitionSource);
    }

  }

  Validator::Validator(
      TranslatedProject &project, const NamingConvention &naming
  ) : mProject(project), mNaming(naming) {}

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

  void Validator::ContainerAnnotation(const Class &cls, const ClassField &field,
                                      const AnnotationDefinition &annotation) const {
    EnforceUniqueAnnotation(cls, field, Annotations::Container);
    ValidateAttributeCount(annotation, Annotations::Container_ElemName, ToString(cls, field));
    if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
      auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
      THROW_IF(underlyingClass == nullptr, "{} is a keyed container of {} which is not a user type",
               ToString(cls, field), field.mType.mTemplateParameters.back().ToString());
      THROW_IF(!underlyingClass->mStruct || !underlyingClass->mStruct->GetIdField(),
               "{} is a keyed container of {} which is not a user type with an id field",
               ToString(cls, field), ToString(*underlyingClass));
      auto key = field.mField->mType.mTemplateParameters.front();
      if (key.mName == "Ref") {
        THROW_IF(key.mTemplateParameters.front().mName != underlyingClass->mStruct->mName,
                 "Keyed container {} of {} should use Ref<{}>",
                 ToString(cls, field), underlyingClass->mStruct->mName, cls.mStruct->mName);
      }
    } else {
      THROW_IF(
          !TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName) &&
          !TypeInfo::Get().CppSets.contains(field.mType.mName),
          "{} should have a container type, found {}", ToString(cls, field), field.mType.mName);
    }
    for (auto &field2: cls.mFields) {
      if (!field2.mField || field2.mField == field.mField)
        continue;
      if (auto container2 = field2.mField->GetAnnotation(Annotations::Container)) {
        auto elemName2 = container2->GetAttribute(Annotations::Container_ElemName);
        THROW_IF(elemName2 &&
                     elemName2->mValue.mName == annotation.GetAttribute(Annotations::Container_ElemName)->mValue.mName,
                 "{} has multiple container fields ({} and {}) with identical elemName: {}",
                 cls.mStruct->mName, ToString(cls, field), ToString(cls, field2), elemName2->mValue.mName);
      }
    }
  }

  void Validator::IndexAnnotation(const Class &cls, const ClassField &field,
                                  const AnnotationDefinition &annotation) const {
    // This doesn't make sense right now since we check index annotation only for containers, added just in case
    THROW_IF(!field.mField || !field.mField->GetAnnotation(Annotations::Container),
             "{} annotation of {} should only be used on container fields",
             annotation.mName, ToString(cls, field));
    ValidateAttributeCount(annotation, Annotations::Index_On, ToString(cls, field));
    ValidateAttributeCount(annotation, Annotations::Index_Using, ToString(cls, field), 0);
    ValidateAttributeCount(annotation, Annotations::Index_ForConverter, ToString(cls, field), 0);
    if (auto indexUsing = annotation.GetAttribute(Annotations::Index_Using)) {
      auto type = Type{mProject.mProject, indexUsing->mValue};
      THROW_IF(!TypeInfo::Get().CppKeyedContainers.contains(type.mName),
               "{} attribute of {} of {} should be a keyed container type, found {}",
               indexUsing->mName, ToString(annotation), ToString(cls, field), type.mName);
    }
    auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
    THROW_IF(!underlyingClass, "{} has an index on {} which is not a user defined struct",
             ToString(cls, field), field.mType.mTemplateParameters.back().ToString());
    auto indexOn = annotation.GetAttribute(Annotations::Index_On);
    auto indexedFieldDefinition = underlyingClass->mStruct->GetField(indexOn->mValue.mName);
    THROW_IF(!indexedFieldDefinition, "{} has an index on non-existent field {} of {}",
             ToString(cls, field), indexOn->mValue.mName, ToString(*underlyingClass));
    auto indexedField = underlyingClass->GetField(mNaming.FieldNameInCpp(*indexedFieldDefinition));
    THROW_IF(!indexedField, "{} has an index on non-existent field {} of {}",
             ToString(cls, field), indexOn->mValue.mName, ToString(*underlyingClass));
    THROW_IF(!TypeInfo::Get().KeyableTypes.contains(indexedField->mType.mName),
             "{} has an index on non-keyable field {}",
             ToString(cls, field), ToString(*underlyingClass, *indexedField));
    THROW_IF(annotation.GetAttribute(Annotations::Index_ForConverter) && !cls.mStruct->GetAnnotation(Annotations::DataManager),
             "{} has an index with a converter but the class is missing the {} annotation",
             ToString(cls, field), Annotations::DataManager);
  }

  void Validator::ValidateAttributeCount(
      const AnnotationDefinition &annotation, const std::string &attributeName,
      const std::string &source, size_t minCount, size_t maxCount) const {
    size_t count = std::count_if(
        annotation.mAttributes.begin(), annotation.mAttributes.end(),
        [&attributeName](const auto &attribute) { return attribute.mName == attributeName; });
    THROW_IF(count < minCount, "Missing {} attribute in {} annotation of {}",
             attributeName, ToString(annotation), source);
    THROW_IF(count > maxCount, "Too many {} attributes in {} annotation of {}",
             attributeName, ToString(annotation), source);
  }

  void Validator::EnforceUniqueAnnotation(
      const Class &cls, const ClassField &field, const std::string &annotationName) const {
    size_t count = std::count_if(
        field.mField->mAnnotations.begin(), field.mField->mAnnotations.end(),
        [&annotationName](const auto &annotation) { return annotation.mName == annotationName; });
    THROW_IF(count != 1, "{} annotation in {} should be used only once",
             annotationName, ToString(cls, field));
  }
}