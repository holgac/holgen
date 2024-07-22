#include "Validator.h"
#include <queue>
#include <set>
#include <string>
#include "TranslatedProject.h"
#include "TypeInfo.h"
#include "core/Annotations.h"
#include "core/Exception.h"
#include "core/St.h"

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

std::string ToString(const StructDefinition &structDefinition) {
  return std::format("{} ({})", structDefinition.mName, structDefinition.mDefinitionSource);
}

std::string ToString(const EnumDefinition &enumDefinition) {
  return std::format("{} ({})", enumDefinition.mName, enumDefinition.mDefinitionSource);
}

std::string ToString(const Class &cls) {
  if (cls.mStruct)
    return ToString(*cls.mStruct);
  else if (cls.mEnum)
    return ToString(*cls.mEnum);
  else
    return std::format("{}", cls.mName);
}

std::string ToString(const Class &cls, const FieldDefinition &fieldDefinition) {
  return std::format("{}.{} ({})", cls.mName, fieldDefinition.mName,
                     fieldDefinition.mDefinitionSource);
}

std::string ToString(const Class &cls, const ClassEnumEntry &entry) {
  if (entry.mEntry)
    return std::format("{}.{} ({})", cls.mName, entry.mName, entry.mEntry->mDefinitionSource);
  else
    return std::format("{}.{}", cls.mName, entry.mName);
}

std::string ToString(const Class &cls, const ClassField &field) {
  if (field.mField)
    return ToString(cls, *field.mField);
  else
    return std::format("{}.{}", cls.mName, field.mName);
}

std::string ToString(const Class &cls, const ClassMethod &method) {
  if (method.mFunction)
    return std::format("{}.{} ({})", cls.mName, method.mFunction->mName,
                       method.mFunction->mDefinitionSource);
  else
    return std::format("{}.{}", cls.mName, method.mName);
}

std::string ToString(const AnnotationDefinition &annotation) {
  return std::format("{} ({})", annotation.mName, annotation.mDefinitionSource);
}

bool AreArgumentsCompatible(const ClassMethodArgument &arg1, const ClassMethodArgument &arg2) {
  if (arg1.mType.mName != arg2.mType.mName)
    return false;
  if (arg1.mType.mType != arg2.mType.mType) {
    if (arg1.mType.mType == PassByType::Pointer || arg2.mType.mType == PassByType::Pointer ||
        arg1.mType.mType == PassByType::MoveReference ||
        arg2.mType.mType == PassByType::MoveReference)
      return false;
  }
  if (arg1.mType.mTemplateParameters.size() != arg2.mType.mTemplateParameters.size())
    return false;
  for (size_t j = 0; j < arg1.mType.mTemplateParameters.size(); ++j) {
    if (arg1.mType.mTemplateParameters[j].mName != arg2.mType.mTemplateParameters[j].mName)
      return false;
  }
  if (arg1.mType.mFunctionalTemplateParameters.size() !=
      arg2.mType.mFunctionalTemplateParameters.size())
    return false;
  for (size_t j = 0; j < arg1.mType.mFunctionalTemplateParameters.size(); ++j) {
    if (arg1.mType.mFunctionalTemplateParameters[j].mName !=
        arg2.mType.mFunctionalTemplateParameters[j].mName)
      return false;
  }
  return true;
}

} // namespace

Validator::Validator(TranslatedProject &project, const NamingConvention &naming) :
    mProject(project), mNaming(naming) {}

void Validator::NewClass(const Class &cls) const {
  THROW_IF(ReservedKeywords.contains(cls.mName), "{} is a reserved keyword", ToString(cls));
  THROW_IF(TypeInfo::Get().CppPrimitives.contains(cls.mName), "{} is a reserved keyword",
           ToString(cls));
  auto dup = mProject.GetClass(cls.mName);
  THROW_IF(dup, "Duplicate class: {} and {}", ToString(*dup), ToString(cls));
}

void Validator::IdField(const Class &cls, const ClassField &field) const {
  NewField(cls, field);
  THROW_IF(!TypeInfo::Get().KeyableTypes.contains(field.mType.mName),
           "{} uses an invalid type for an id: {}", ToString(cls, field), field.mType.mName);
  auto dup = cls.GetIdField();
  THROW_IF(dup, "{} has multiple id fields: {} and {}", ToString(cls), ToString(cls, *dup),
           ToString(cls, field));
}

void Validator::NewField(const Class &cls, const ClassField &field) const {
  THROW_IF(ReservedKeywords.contains(field.mName), "{} is a reserved keyword",
           ToString(cls, field));
  THROW_IF(TypeInfo::Get().CppPrimitives.contains(field.mName), "{} is a reserved keyword",
           ToString(cls, field));
  auto dup = cls.GetField(field.mName);
  THROW_IF(dup, "Duplicate field: {} and {}", ToString(cls, *dup), ToString(cls, field));
  ValidateType(field.mType, cls, false, nullptr, ToString(cls, field));
}

void Validator::ValidateType(const Type &type, const Class &cls, bool acceptVoid,
                             const ClassMethod *method, const std::string &source) const {
  auto isTemplateParameter = [&](const std::string &name) {
    return cls.GetTemplateParameter(name) || (method && method->GetTemplateParameter(name));
  };
  if (TypeInfo::Get().CppBasicTypes.contains(type.mName)) {
    THROW_IF(type.mTemplateParameters.size() > 0,
             "Primitive type {} used by {} cannot have template parameters", type.mName, source);
    THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
             "Primitive type {} used by {} cannot have functional template parameters", type.mName,
             source);
  } else if (TypeInfo::Get().CppLists.contains(type.mName) ||
             TypeInfo::Get().CppIndexedContainers.contains(type.mName)) {
    THROW_IF(type.mTemplateParameters.size() != 1 &&
                 !TypeInfo::Get().CppFixedSizeContainers.contains(type.mName),
             "Container type {} used by {} should have a single template parameter", type.mName,
             source);
    THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
             "Container type {} used by {} cannot have functional template arguments", type.mName,
             source);
    ValidateType(type.mTemplateParameters.front(), cls, false, method, source);
  } else if (TypeInfo::Get().CppSets.contains(type.mName)) {
    THROW_IF(type.mTemplateParameters.size() != 1,
             "Set type {} used by {} should have a single template parameter", type.mName, source);
    THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
             "Set type {} used by {} cannot have functional template arguments", type.mName,
             source);
    THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mTemplateParameters[0].mName) &&
                 !isTemplateParameter(type.mTemplateParameters[0].mName),
             "Set type {} used by {} should have a keyable template parameter, found {}",
             type.mName, source, type.mTemplateParameters[0].mName);
    ValidateType(type.mTemplateParameters[0], cls, false, method, source);
  } else if (TypeInfo::Get().CppKeyedContainers.contains(type.mName)) {
    THROW_IF(type.mTemplateParameters.size() != 2,
             "Map type {} used by {} should have two template parameters", type.mName, source);
    THROW_IF(type.mFunctionalTemplateParameters.size() > 0,
             "Map type {} used by {} cannot have functional template arguments", type.mName,
             source);
    THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mTemplateParameters[0].mName) &&
                 !isTemplateParameter(type.mTemplateParameters[0].mName) &&
                 !mProject.mProject.GetEnum(type.mTemplateParameters[0].mName),
             "Map type {} used by {} should have a keyable first template parameter, found {}",
             type.mName, source, type.mTemplateParameters[0].mName);
    ValidateType(type.mTemplateParameters[1], cls, false, method, source);
  } else if (type.mName == "void") {
    THROW_IF(!acceptVoid && type.mType != PassByType::Pointer, "Invalid void usage in {}", source);
    THROW_IF(type.mType == PassByType::Reference || type.mType == PassByType::MoveReference,
             "Void cannot be passed by reference in {}", source);
    THROW_IF(type.mTemplateParameters.size() > 0, "Void cannot have template parameters in {}",
             source);
  } else if (mProject.GetClass(type.mName)) {
    // all good
  } else if (isTemplateParameter(type.mName)) {
    // all good
  } else if (auto usingStatement = cls.GetUsing(type.mName)) {
    ValidateType(usingStatement->mSourceType, cls, acceptVoid, method, source);
  } else if (type.mName.starts_with(cls.mName + "::")) {
    auto rest = type.mName.substr(cls.mName.size() + 2);
    THROW_IF(!cls.GetUsing(rest) && !cls.GetNestedEnum(rest),
             "Class-defined type {} does not exist in {}", type.mName, source);
  } else {
    THROW_IF(!TypeInfo::Get().AllowlistedTypes.contains(type.mName), "Unknown type {} used by {}",
             type.mName, source);
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
    bool argumentsAreCompatible = true;
    for (auto it1 = method.mArguments.begin(), it2 = method2.mArguments.begin();
         it1 != method.mArguments.end(); ++it1, ++it2) {
      if (!AreArgumentsCompatible(*it1, *it2)) {
        argumentsAreCompatible = false;
        break;
      }
    }
    THROW_IF(argumentsAreCompatible, "Duplicate method: {} and {}", ToString(cls, method2),
             ToString(cls, method));
  }
  ValidateType(method.mReturnType, cls, true, &method, ToString(cls, method));
  for (auto &arg: method.mArguments) {
    ValidateType(arg.mType, cls, false, &method, ToString(cls, method));
  }
  THROW_IF(method.mConstness == Constness::Const && method.mStaticness == Staticness::Static,
           "Const static method: {}", ToString(cls, method));
  if (method.mFunction && method.mFunction->GetAnnotation(Annotations::Func) &&
      method.mFunction->GetAnnotation(Annotations::Func)
          ->GetAttribute(Annotations::Func_OnDestroy)) {
    THROW_IF(!method.mArguments.empty(),
             "{} has {} attribute which does not support functions with arguments",
             ToString(cls, method), Annotations::Func_OnDestroy);
    THROW_IF(method.mReturnType.mName != "void",
             "{} has {} attribute which does not support functions that return a value",
             ToString(cls, method), Annotations::Func_OnDestroy);
  }
}

void Validator::ContainerAnnotation(const Class &cls, const ClassField &field,
                                    const AnnotationDefinition &annotation) const {
  EnforceUniqueAnnotation(cls, field, Annotations::Container);
  ValidateAttributeCount(annotation, Annotations::Container_ElemName, ToString(cls, field));
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
    // TODO: why? can't have a container of strings or vectors?
    THROW_IF(underlyingClass == nullptr, "{} is a keyed container of {} which is not a user type",
             ToString(cls, field), field.mType.mTemplateParameters.back().ToString(true));
    THROW_IF(!underlyingClass->mStruct || !underlyingClass->GetIdField(),
             "{} is a keyed container of {} which is not a user type with an id field",
             ToString(cls, field), ToString(*underlyingClass));
    auto key = field.mField->mType.mTemplateParameters.front();
    if (key.mName == "Ref") {
      THROW_IF(key.mTemplateParameters.front().mName != underlyingClass->mStruct->mName,
               "Keyed container {} of {} should use Ref<{}>", ToString(cls, field),
               underlyingClass->mStruct->mName, cls.mStruct->mName);
    }
  } else {
    THROW_IF(!TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName) &&
                 !TypeInfo::Get().CppSets.contains(field.mType.mName),
             "{} should have a container type, found {}", ToString(cls, field), field.mType.mName);
  }
  for (auto &field2: cls.mFields) {
    if (!field2.mField || field2.mField == field.mField)
      continue;
    if (auto container2 = field2.mField->GetAnnotation(Annotations::Container)) {
      auto elemName2 = container2->GetAttribute(Annotations::Container_ElemName);
      THROW_IF(elemName2 &&
                   elemName2->mValue.mName ==
                       annotation.GetAttribute(Annotations::Container_ElemName)->mValue.mName,
               "{} has multiple container fields ({} and {}) with identical elemName: {}",
               cls.mStruct->mName, ToString(cls, field), ToString(cls, field2),
               elemName2->mValue.mName);
    }
  }
}

void Validator::ManagedAnnotation(const Class &cls, const AnnotationDefinition &annotation) const {
  THROW_IF(!cls.GetIdField(), "{} is a managed class without an id field", ToString(cls));
  EnforceUniqueAnnotation(cls, Annotations::Managed);
  ValidateAttributeCount(annotation, Annotations::Managed_By, ToString(cls));
  ValidateAttributeCount(annotation, Annotations::Managed_Field, ToString(cls));
  auto dataManager =
      mProject.GetClass(annotation.GetAttribute(Annotations::Managed_By)->mValue.mName);
  THROW_IF(!dataManager || !dataManager->mStruct, "{} is managed by {} which does not exist",
           ToString(cls), annotation.GetAttribute(Annotations::Managed_By)->mValue.mName);
  THROW_IF(!dataManager->mStruct->GetAnnotation(Annotations::DataManager),
           "{} is managed by {} which is not a data manager", ToString(cls),
           ToString(*dataManager));
  auto containerField = dataManager->GetFieldFromDefinitionName(
      annotation.GetAttribute(Annotations::Managed_Field)->mValue.mName);
  THROW_IF(!containerField, "{} is managed by {} field of {} which does not exist", ToString(cls),
           annotation.GetAttribute(Annotations::Managed_Field)->mValue.mName,
           ToString(*dataManager));
  THROW_IF(!containerField->mField->GetAnnotation(Annotations::Container),
           "{} is managed by {} which is not a container", ToString(cls),
           ToString(*dataManager, *containerField->mField));
  THROW_IF(containerField->mField->mType.mTemplateParameters.back().mName != cls.mStruct->mName,
           "{} is managed by {} which is not a container of {}", ToString(cls),
           ToString(*dataManager, *containerField), cls.mName);
}

void Validator::IndexAnnotation(const Class &cls, const ClassField &field,
                                const AnnotationDefinition &annotation) const {
  // This doesn't make sense right now since we check index annotation only for containers, added
  // just in case
  THROW_IF(!field.mField || !field.mField->GetAnnotation(Annotations::Container),
           "{} annotation of {} should only be used on container fields", annotation.mName,
           ToString(cls, field));
  ValidateAttributeCount(annotation, Annotations::Index_On, ToString(cls, field));
  ValidateAttributeCount(annotation, Annotations::Index_Using, ToString(cls, field), 0);
  ValidateAttributeCount(annotation, Annotations::Index_ForConverter, ToString(cls, field), 0);
  if (auto indexUsing = annotation.GetAttribute(Annotations::Index_Using)) {
    auto type = Type{mProject, indexUsing->mDefinitionSource, indexUsing->mValue};
    THROW_IF(!TypeInfo::Get().CppKeyedContainers.contains(type.mName),
             "{} attribute of {} of {} should be a keyed container type, found {}",
             indexUsing->mName, ToString(annotation), ToString(cls, field), type.mName);
  }
  auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
  THROW_IF(!underlyingClass, "{} has an index on {} which is not a user defined struct",
           ToString(cls, field), field.mType.mTemplateParameters.back().ToString(true));
  auto indexOn = annotation.GetAttribute(Annotations::Index_On);
  auto indexedField = underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);
  THROW_IF(!indexedField, "{} has an index on non-existent field {} of {}", ToString(cls, field),
           indexOn->mValue.mName, ToString(*underlyingClass));
  THROW_IF(!TypeInfo::Get().KeyableTypes.contains(indexedField->mType.mName),
           "{} has an index on non-keyable field {}", ToString(cls, field),
           ToString(*underlyingClass, *indexedField));
  THROW_IF(annotation.GetAttribute(Annotations::Index_ForConverter) &&
               !cls.mStruct->GetAnnotation(Annotations::DataManager),
           "{} has an index with a converter but the class is missing the {} annotation",
           ToString(cls, field), Annotations::DataManager);
}

void Validator::ValidateAttributeCount(const AnnotationDefinition &annotation,
                                       const std::string &attributeName, const std::string &source,
                                       size_t minCount, size_t maxCount) const {
  size_t count = std::count_if(
      annotation.mAttributes.begin(), annotation.mAttributes.end(),
      [&attributeName](const auto &attribute) { return attribute.mName == attributeName; });
  THROW_IF(count < minCount, "Missing {} attribute in {} annotation of {}", attributeName,
           ToString(annotation), source);
  THROW_IF(count > maxCount, "Too many {} attributes in {} annotation of {}", attributeName,
           ToString(annotation), source);
}

void Validator::EnforceUniqueAnnotation(const Class &cls, const std::string &annotationName) const {
  size_t count = std::count_if(
      cls.mStruct->mAnnotations.begin(), cls.mStruct->mAnnotations.end(),
      [&annotationName](const auto &annotation) { return annotation.mName == annotationName; });
  THROW_IF(count != 1, "{} annotation in {} should be used only once", annotationName,
           ToString(cls));
}

void Validator::EnforceUniqueAnnotation(const Class &cls, const ClassField &field,
                                        const std::string &annotationName) const {
  size_t count = std::count_if(
      field.mField->mAnnotations.begin(), field.mField->mAnnotations.end(),
      [&annotationName](const auto &annotation) { return annotation.mName == annotationName; });
  THROW_IF(count != 1, "{} annotation in {} should be used only once", annotationName,
           ToString(cls, field));
}

void Validator::Enum(const Class &cls) const {
  THROW_IF(!cls.mEnum, "{} is not an enum", ToString(cls));
}

void Validator::NewEnumEntry(const Class &cls, const ClassEnum &nestedEnum,
                             const ClassEnumEntry &entry) const {
  auto existingEntry = nestedEnum.GetEntry(entry.mName);
  THROW_IF(existingEntry, "Enum {} contains duplicate entries {} and {}", ToString(cls),
           ToString(cls, *existingEntry), ToString(cls, entry));
}

namespace {
struct ConverterSpec {
  const Class &mClass;
  const ClassField &mField;
  const TypeDefinition &mFromType;
  const TypeDefinition *mToType;
};
} // namespace

void Validator::JsonConverters() const {
  std::map<std::string, ConverterSpec> converters;
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct)
      continue;
    for (auto &field: cls.mFields) {
      if (!field.mField || !field.mField->GetAnnotation(Annotations::JsonConvert))
        continue;
      auto converter = field.mField->GetAnnotation(Annotations::JsonConvert);
      ValidateAttributeCount(*converter, Annotations::JsonConvert_From, ToString(cls, field));
      ValidateAttributeCount(*converter, Annotations::JsonConvert_Using, ToString(cls, field));
      auto cs =
          ConverterSpec{cls, field, converter->GetAttribute(Annotations::JsonConvert_From)->mValue,
                        &field.mField->mType};
      auto convertElem = converter->GetAttribute(Annotations::JsonConvert_Elem);
      if (convertElem)
        cs.mToType = &field.mField->mType.mTemplateParameters.back();
      auto convertKey = converter->GetAttribute(Annotations::JsonConvert_Key);
      if (convertKey)
        cs.mToType = &field.mField->mType.mTemplateParameters.front();
      auto key = converter->GetAttribute(Annotations::JsonConvert_Using)->mValue.mName;
      auto [it, res] = converters.try_emplace(key, cs);
      if (!res) {
        THROW_IF(
            cs.mFromType != it->second.mFromType,
            "Json converter {} is used by {} and {} with different source types: {} and {}", key,
            ToString(it->second.mClass, it->second.mField), ToString(cls, field),
            Type{mProject, field.mField->mDefinitionSource, it->second.mFromType}.ToString(true),
            Type{mProject, field.mField->mDefinitionSource, cs.mFromType}.ToString(true));
        THROW_IF(
            *cs.mToType != *it->second.mToType,
            "Json converter {} is used by {} and {} with different target types: {} and {}", key,
            ToString(it->second.mClass, it->second.mField), ToString(cls, field),
            Type{mProject, field.mField->mDefinitionSource, *it->second.mToType}.ToString(true),
            Type{mProject, field.mField->mDefinitionSource, *cs.mToType}.ToString(true));
      }
    }
  }

  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct)
      continue;
    for (auto &field: cls.mFields) {
      if (!field.mField || !field.mField->GetAnnotation(Annotations::Index) ||
          !field.mField->GetAnnotation(Annotations::Index)
               ->GetAttribute(Annotations::Index_ForConverter))
        continue;
      auto index = field.mField->GetAnnotation(Annotations::Index);
      auto converter = field.mField->GetAnnotation(Annotations::Index)
                           ->GetAttribute(Annotations::Index_ForConverter);
      auto it = converters.find(converter->mValue.mName);
      THROW_IF(it == converters.end(), "{} of {} references non-existent converter {}",
               ToString(*field.mField->GetAnnotation(Annotations::Index)), ToString(cls, field),
               converter->mValue.mName);

      auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
      auto indexField = underlyingClass->GetFieldFromDefinitionName(
          index->GetAttribute(Annotations::Index_On)->mValue.mName);
      auto underlyingIdField = underlyingClass->GetIdField();
      auto existingFromTypeName =
          Type{mProject, field.mField->mDefinitionSource, it->second.mFromType}.ToString(true);
      auto existingToTypeName =
          Type{mProject, field.mField->mDefinitionSource, *it->second.mToType}.ToString(true);
      auto fromTypeName = indexField->mType.ToString(true);
      std::string toTypeName = "size_t";
      if (underlyingIdField)
        toTypeName =
            Type{mProject, field.mField->mDefinitionSource, underlyingIdField->mField->mType}
                .ToString(true);
      // TODO: tolerate mismatches of integral types?
      THROW_IF(fromTypeName != existingFromTypeName,
               "{} of {} references converter {} with different source type than {}: {} and {}",
               ToString(*field.mField->GetAnnotation(Annotations::Index)), ToString(cls, field),
               converter->mValue.mName, ToString(it->second.mClass, it->second.mField),
               fromTypeName, existingFromTypeName);
      THROW_IF(toTypeName != existingToTypeName,
               "{} of {} references converter {} with different target type than {}: {} and {}",
               ToString(*field.mField->GetAnnotation(Annotations::Index)), ToString(cls, field),
               converter->mValue.mName, ToString(it->second.mClass, it->second.mField), toTypeName,
               existingToTypeName);
    }
  }
}

} // namespace holgen
