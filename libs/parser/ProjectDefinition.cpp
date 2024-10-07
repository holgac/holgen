#include "ProjectDefinition.h"
#include <optional>
#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"
#include "holgen.h"

namespace holgen {
namespace {
template <bool returnAttribute, typename Ret, typename T>
const Ret *AnnotationMatcher(const T &container, const std::string &annotationName,
                             const std::string &attributeName,
                             const std::optional<std::string> &attributeValue) {
  for (auto &annotation: container.mAnnotations) {
    if (annotation.mName != annotationName) {
      continue;
    }
    for (auto &attribute: annotation.mAttributes) {
      if (attribute.mName == attributeName) {
        if (!attributeValue.has_value() || attribute.mValue.mName == *attributeValue) {
          if constexpr (returnAttribute) {
            return &attribute;
          } else {
            return &annotation;
          }
        }
      }
    }
  }
  return nullptr;
}
} // namespace

const AnnotationAttributeDefinition *
    AnnotationsMixin::GetMatchingAttribute(const std::string &annotationName,
                                           const std::string &attributeName,
                                           const std::optional<std::string> &attributeValue) const {
  return AnnotationMatcher<true, AnnotationAttributeDefinition>(*this, annotationName,
                                                                attributeName, attributeValue);
}

const AnnotationDefinition *AnnotationsMixin::GetMatchingAnnotation(
    const std::string &annotationName, const std::string &attributeName,
    const std::optional<std::string> &attributeValue) const {
  return AnnotationMatcher<false, AnnotationDefinition>(*this, annotationName, attributeName,
                                                        attributeValue);
};

// namespace

const FieldDefinition *StructDefinition::GetIdField() const {
  for (const auto &field: mFields) {
    if (field.GetAnnotation(Annotations::Id))
      return &field;
  }
  return nullptr;
}

const FunctionDefinition *
    StructDefinition::GetHashFunction(const ProjectDefinition &project) const {
  for (auto &function: mFunctions) {
    if (function.GetMatchingAttribute(Annotations::Func, Annotations::Func_Hash)) {
      return &function;
    }
  }

  for (auto &mixin: mMixins) {
    auto func = project.GetStruct(mixin)->GetHashFunction(project);
    if (func) {
      return func;
    }
  }

  return nullptr;
}

const FunctionDefinition *
    StructDefinition::GetToStringFunction(const ProjectDefinition &project) const {
  for (auto &function: mFunctions) {
    if (function.GetMatchingAttribute(Annotations::Func, Annotations::Func_ToString)) {
      return &function;
    }
  }

  for (auto &mixin: mMixins) {
    auto func = project.GetStruct(mixin)->GetToStringFunction(project);
    if (func) {
      return func;
    }
  }

  return nullptr;
}

bool TypeDefinition::operator==(const TypeDefinition &rhs) const {
  if (mName != rhs.mName)
    return false;
  if (mTemplateParameters.size() != rhs.mTemplateParameters.size())
    return false;
  for (size_t i = 0; i < mTemplateParameters.size(); ++i) {
    if (mTemplateParameters[i] != rhs.mTemplateParameters[i])
      return false;
  }
  return true;
}

GEN_GETTER_BY_NAME(AnnotationDefinition, AnnotationAttributeDefinition, GetAttribute, mAttributes)

GEN_GETTER_BY_NAME(AnnotationsMixin, AnnotationDefinition, GetAnnotation, mAnnotations)

GEN_GETTER_BY_NAME(StructDefinition, FieldDefinition, GetField, mFields);

GEN_GETTER_BY_NAME(StructDefinition, FunctionDefinition, GetFunction, mFunctions);

GEN_GETTER_BY_NAME(ProjectDefinition, EnumDefinition, GetEnum, mEnums);

GEN_GETTER_BY_NAME(ProjectDefinition, StructDefinition, GetStruct, mStructs);

GEN_GETTER_BY_NAME(EnumDefinition, EnumEntryDefinition, GetEnumEntry, mEntries);

const EnumEntryDefinition *EnumDefinition::GetDefaultEntry() const {
  const EnumEntryDefinition *defaultEntry = nullptr;
  for (auto &entry: mEntries) {
    if (entry.GetMatchingAttribute(Annotations::Enum, Annotations::Enum_Default)) {
      THROW_IF(defaultEntry, "Enum {} has multiple default entries: {} ({}) and {} ({})", mName,
               defaultEntry->mName, defaultEntry->mDefinitionSource, entry.mName,
               entry.mDefinitionSource)
      defaultEntry = &entry;
    }
  }
  return defaultEntry;
};
} // namespace holgen
