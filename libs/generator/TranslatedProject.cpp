#include "TranslatedProject.h"
#include "core/Annotations.h"
#include "holgen.h"

namespace holgen {

TranslatedProject::TranslatedProject(const ProjectDefinition &projectDefinition) :
    mProject(projectDefinition), mDependencyGraph(mProject) {}

ClassMethod *Class::GetMethod(const std::string &name, Constness constness) {
  for (auto &method: mMethods) {
    if (method.mName == name && method.mConstness == constness)
      return &method;
  }
  return nullptr;
}

const Using *Class::GetUsing(const std::string &name) const {
  for (auto &usingStatement: mUsings) {
    if (usingStatement.mTargetType == name)
      return &usingStatement;
  }
  return nullptr;
}

GEN_GETTER_BY_NAME(TranslatedProject, Class, GetClass, mClasses)

GEN_GETTER_BY_NAME_NONCONST(TranslatedProject, Class, GetClass, mClasses);

GEN_GETTER_BY_NAME(ClassMethod, TemplateParameter, GetTemplateParameter, mTemplateParameters)

GEN_GETTER_BY_NAME(ClassEnum, ClassEnumEntry, GetEntry, mEntries)

GEN_GETTER_BY_NAME_NONCONST(Class, ClassField, GetField, mFields);

GEN_GETTER_BY_NAME(Class, ClassField, GetField, mFields)

GEN_GETTER_BY_NAME(Class, TemplateParameter, GetTemplateParameter, mTemplateParameters)

GEN_GETTER_BY_NAME(Class, ClassEnum, GetNestedEnum, mNestedEnums)

ClassField *Class::GetFieldFromDefinitionName(const std::string &name) {
  for (auto &field: mFields) {
    if (field.mField && field.mField->mName == name) {
      return &field;
    }
  }
  return nullptr;
}

const ClassField *Class::GetIdField() const {
  for (auto &field: mFields) {
    if (field.mField && field.mField->GetAnnotation(Annotations::Id))
      return &field;
  }
  return nullptr;
}

TemplateParameter::TemplateParameter(std::string type, std::string name) :
    mType(std::move(type)), mName(std::move(name)) {}

std::vector<std::pair<Class *, const EnumEntryDefinition *>>
    TranslatedProject::GetVariantClassesOfEnum(const std::string &enumName) {
  std::vector<std::pair<Class *, const EnumEntryDefinition *>> result;
  auto &enumDefinition = *mProject.GetEnum(enumName);
  for (auto &cls: mClasses) {
    if (!cls.mStruct || cls.mStruct->mIsMixin)
      continue;
    if (!cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Enum, enumName))
      continue;
    auto entry = cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Entry)->mValue.mName;
    result.emplace_back(&cls, enumDefinition.GetEnumEntry(entry));
    ;
  }
  std::sort(result.begin(), result.end(),
            [](const auto &lhs, const auto &rhs) { return lhs.first->mName < rhs.first->mName; });
  return result;
}
} // namespace holgen
