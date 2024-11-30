#include "CppTypes.h"
#include "holgen.h"
#include "core/Annotations.h"

#include "generator/utils/NamingConvention.h"

namespace holgen {

const ClassMethod *Class::GetMethod(const std::string &name, Constness constness) const {
  for (auto &method: mMethods) {
    if (method.mName == name && method.mConstness == constness)
      return &method;
  }
  return nullptr;
}

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

bool Class::IsProxyable() const {
  if (mEnum)
    return false;
  return !mStruct ||
      (!mStruct->GetMatchingAttribute(Annotations::Script, Annotations::Script_AlwaysMirror) &&
       !mStruct->GetAnnotation(Annotations::Singleton));
}

GEN_GETTER_BY_NAME(ClassMethod, TemplateParameter, GetTemplateParameter, mTemplateParameters)

bool ClassMethod::IsStatic(const Class &cls) const {
  if (mStaticness == Staticness::Static)
    return true;
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Singleton))
    return true;
  return false;
}

bool ClassDestructor::IsEmpty() const {
  return mBody.IsEmpty() && mDefaultDelete == DefaultDelete::Neither;
}

GEN_GETTER_BY_NAME(ClassEnum, ClassEnumEntry, GetEntry, mEntries)

std::string ClassEnum::GetUnderlyingType(const Class &cls) const {
  if (auto usingStatement = cls.GetUsing(mUnderlyingType))
    return usingStatement->mSourceType.mName;
  return mUnderlyingType;
}

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

bool Class::HasUserDefinedMethods() const {
  for (const auto &method: mMethods) {
    if (method.mUserDefined && method.mVirtuality != Virtuality::PureVirtual) {
      return true;
    }
  }
  for (const auto &ctor: mConstructors) {
    if (ctor.mUserDefined) {
      return true;
    }
  }
  return false;
}

ClassField *Class::GetFieldFromDefinitionName(const std::string &name) {
  for (auto &field: mFields) {
    if (field.mField && field.mField->mName == name) {
      return &field;
    }
  }
  return nullptr;
}

const ClassField *Class::GetFieldFromDefinitionName(const std::string &name) const {
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

bool ClassField::IsVariantTypeField(const Class &cls, const std::string **rawName,
                                    const NamingConvention &naming) const {
  for (auto &classField: cls.mFields) {
    if (!classField.mField || !classField.mField->GetAnnotation(Annotations::Variant))
      continue;
    auto variantAnnotation = classField.mField->GetAnnotation(Annotations::Variant);
    auto variantFieldAttribute = variantAnnotation->GetAttribute(Annotations::Variant_TypeField);
    if (naming.FieldNameInCpp(variantFieldAttribute->mValue.mName) == mName) {
      if (rawName) {
        *rawName = &variantFieldAttribute->mValue.mName;
      }
      return true;
    }
  }
  return false;
}

TemplateParameter::TemplateParameter(std::string type, std::string name) :
    mType(std::move(type)), mName(std::move(name)) {}
} // namespace holgen
