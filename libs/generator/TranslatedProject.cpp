#include "TranslatedProject.h"
#include "holgen.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

Class::VariantData Class::GetVariantData() {
  VariantData variantTypeFieldToVariantFields;
  for (auto &field: mFields) {
    if (!field.mField || !field.mField->GetAnnotation(Annotations::Variant)) {
      continue;
    }
    auto typeField = field.mField->GetAnnotation(Annotations::Variant)
                         ->GetAttribute(Annotations::Variant_TypeField)
                         ->mValue.mName;
    variantTypeFieldToVariantFields[typeField].push_back(&field);
  }
  return variantTypeFieldToVariantFields;
}

bool Class::HasVirtualMethods() const {
  for (auto &method: mMethods) {
    if (method.mVirtuality != Virtuality::NotVirtual) {
      return true;
    }
  }
  return false;
}

bool Class::IsAbstract() const {
  for (auto &method: mMethods) {
    if (method.mVirtuality == Virtuality::PureVirtual) {
      return true;
    }
  }
  return false;
}

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

bool ClassDestructor::IsEmpty() const {
  return mBody.IsEmpty() && mDefaultDelete == DefaultDelete::Neither;
}

GEN_GETTER_BY_NAME(ClassEnum, ClassEnumEntry, GetEntry, mEntries)

GEN_GETTER_BY_NAME_NONCONST(Class, ClassField, GetField, mFields);

GEN_GETTER_BY_NAME(Class, ClassField, GetField, mFields)

GEN_GETTER_BY_NAME(Class, TemplateParameter, GetTemplateParameter, mTemplateParameters)

GEN_GETTER_BY_NAME(Class, ClassEnum, GetNestedEnum, mNestedEnums)

ClassConstructor *Class::GetMoveConstructor() {
  for (auto &ctor: mConstructors) {
    if (ctor.mArguments.size() != 1)
      continue;
    if (ctor.mArguments.front().mType != Type{mName, PassByType::MoveReference})
      continue;
    return &ctor;
  }
  return nullptr;
}

ClassConstructor *Class::GetCopyConstructor() {
  for (auto &ctor: mConstructors) {
    if (ctor.mArguments.size() != 1)
      continue;
    if (ctor.mArguments.front().mType != Type{mName, PassByType::Reference, Constness::Const})
      continue;
    return &ctor;
  }
  return nullptr;
}

ClassConstructor *Class::GetDefaultConstructor() {
  for (auto &ctor: mConstructors) {
    if (ctor.mArguments.size() == 0) {
      return &ctor;
    }
  }
  return nullptr;
}

ClassMethod *Class::GetMoveAssignment() {
  for (auto &method: mMethods) {
    if (method.mName != "operator=")
      continue;
    if (method.mArguments.size() != 1)
      continue;
    if (method.mArguments.front().mType != Type{mName, PassByType::MoveReference})
      continue;
    return &method;
  }
  return nullptr;
}

ClassMethod *Class::GetCopyAssignment() {
  for (auto &method: mMethods) {
    if (method.mName != "operator=")
      continue;
    if (method.mArguments.size() != 1)
      continue;
    if (method.mArguments.front().mType != Type{mName, PassByType::Reference, Constness::Const})
      continue;
    return &method;
  }
  return nullptr;
}

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
    if (!cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Enum,
                                           enumName))
      continue;
    auto entry = cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Entry)
                     ->mValue.mName;
    auto enumEntry = enumDefinition.GetEnumEntry(entry);
    THROW_IF(!enumEntry, "Entry {} of enum {} ({}) referenced by {} ({}) does not exist!", entry,
             enumDefinition.mName, enumDefinition.mDefinitionSource, cls.mName,
             cls.mStruct->mDefinitionSource);
    result.emplace_back(&cls, enumDefinition.GetEnumEntry(entry));
    ;
  }
  std::sort(result.begin(), result.end(),
            [](const auto &lhs, const auto &rhs) { return lhs.first->mName < rhs.first->mName; });
  return result;
}
} // namespace holgen
