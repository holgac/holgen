#include "ClassFieldVariantPlugin.h"

#include "core/Annotations.h"
#include "core/Exception.h"
#include "core/St.h"

namespace holgen {
void ClassFieldVariantPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    ProcessStructDefinition(cls, *cls.mStruct);
  }
}

void ClassFieldVariantPlugin::ProcessStructDefinition(Class &cls,
                                                      const StructDefinition &structDefinition) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
  }
  std::set<std::string> variantTypeFields;
  for (auto &fieldDefinition: structDefinition.mFields) {
    if (fieldDefinition.mType.mName != St::Variant)
      continue;
    ProcessVariantField(cls, fieldDefinition);
    variantTypeFields.insert(fieldDefinition.GetAnnotation(Annotations::Variant)
                                 ->GetAttribute(Annotations::Variant_TypeField)
                                 ->mValue.mName);
  }
  for (auto &variantType: variantTypeFields) {
    ProcessVariantType(cls, variantType);
  }
}

void ClassFieldVariantPlugin::ProcessVariantField(Class &cls,
                                                  const FieldDefinition &fieldDefinition) {
  // TODO: validate variant annotations
  auto variantAnnotation = fieldDefinition.GetAnnotation(Annotations::Variant);
  THROW_IF(!variantAnnotation, "variant fields should have a variant annotation!");
  auto typeAttribute = variantAnnotation->GetAttribute(Annotations::Variant_TypeField);
  THROW_IF(!typeAttribute, "Variant annotation missing type in {}",
           variantAnnotation->mDefinitionSource);
  auto &type = typeAttribute->mValue.mName;
  auto enumAttribute = variantAnnotation->GetAttribute(Annotations::Variant_Enum);
  auto existingTypeField = cls.GetField(Naming().FieldNameInCpp(type));
  THROW_IF(!enumAttribute && !existingTypeField,
           "{} defines an implicit type enum without specifying the type",
           variantAnnotation->mDefinitionSource);
  THROW_IF(enumAttribute && existingTypeField &&
               enumAttribute->mValue.mName != existingTypeField->mType.mName,
           "{} uses enum {} for type field {} which was previously defined as {}",
           variantAnnotation->mDefinitionSource, enumAttribute->mValue.mName,
           existingTypeField->mName, existingTypeField->mType.mName);

  auto &enumName = enumAttribute ? enumAttribute->mValue.mName : existingTypeField->mType.mName;
  if (!existingTypeField) {
    auto typeField = ClassField{Naming().FieldNameInCpp(type), Type{enumName}};
    Validate().NewField(cls, typeField);
    cls.mFields.emplace_back(std::move(typeField));
  }

  std::stringstream arraySizeSpecifier;
  auto dataField = ClassField{Naming().FieldNameInCpp(fieldDefinition), Type{"std::array"}};
  bool isFirst = true;
  auto matchingClasses = mProject.GetVariantClassesOfEnum(enumName);
  THROW_IF(matchingClasses.empty(), "Variant field with no matching struct in {}.{} ({})",
           cls.mName, fieldDefinition.mName, fieldDefinition.mDefinitionSource);
  for (auto &[matchingClass, enumEntry]: matchingClasses) {
    // TODO: don't allow variant structs to be inherited from
    auto &projectStruct = *matchingClass->mStruct;
    if (isFirst) {
      isFirst = false;
    } else {
      arraySizeSpecifier << ", ";
    }
    cls.mHeaderIncludes.AddLocalHeader(projectStruct.mName + ".h");
    arraySizeSpecifier << "sizeof(" << projectStruct.mName << ")";
    auto entryStr = std::format("{}::{}", enumName, enumEntry->mName);
    for (int i = 0; i < 2; ++i) {
      auto constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto method = ClassMethod{Naming().VariantGetterNameInCpp(fieldDefinition, projectStruct),
                                Type{projectStruct.mName, PassByType::Pointer, constness},
                                Visibility::Public, constness};
      method.mBody.Add(
          R"R(HOLGEN_FAIL_IF({} != {}, "Attempting to get {}.{} as {} while its actual type is {{}}!", {});)R",
          Naming().FieldNameInCpp(type), entryStr, cls.mName, fieldDefinition.mName,
          projectStruct.mName, Naming().FieldNameInCpp(type));
      method.mBody.Add("return reinterpret_cast<{}>({}.data());", method.mReturnType.ToString(true),
                       dataField.mName);
      Validate().NewMethod(cls, method);
      cls.mMethods.push_back(std::move(method));
    }
  }

  dataField.mType.mTemplateParameters.emplace_back("uint8_t");
  if (matchingClasses.size() > 1)
    dataField.mType.mTemplateParameters.emplace_back("std::max({" + arraySizeSpecifier.str() +
                                                     "})");
  else
    dataField.mType.mTemplateParameters.emplace_back(arraySizeSpecifier.str());
  dataField.mField = &fieldDefinition;
  Validate().NewField(cls, dataField);
  cls.mFields.emplace_back(std::move(dataField));
  cls.mHeaderIncludes.AddStandardHeader("algorithm");
}

void ClassFieldVariantPlugin::ProcessVariantType(Class &cls, const std::string &typeFieldName) {
  auto typeField = cls.GetField(Naming().FieldNameInCpp(typeFieldName));

  if (auto setter =
          cls.GetMethod(Naming().FieldSetterNameInCpp(typeFieldName), Constness::NotConst)) {
    setter->mBody = {};
    ProcessVariantTypeCommon(cls, typeFieldName, *setter, VariantTypeProcessType::Setter);
  } else {
    auto method = ClassMethod{Naming().FieldSetterNameInCpp(typeFieldName), Type{"void"},
                              Visibility::Public, Constness::NotConst};
    auto &arg = method.mArguments.emplace_back("val", typeField->mType);
    arg.mType.PreventCopying();
    ProcessVariantTypeCommon(cls, typeFieldName, method, VariantTypeProcessType::Setter);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  {
    auto method = ClassMethod{Naming().VariantResetterNameInCpp(typeFieldName), Type{"void"},
                              Visibility::Public, Constness::NotConst};
    ProcessVariantTypeCommon(cls, typeFieldName, method, VariantTypeProcessType::Resetter);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  if (!cls.GetMethod(Naming().FieldGetterNameInCpp(typeFieldName), Constness::Const)) {
    auto method = ClassMethod{Naming().FieldGetterNameInCpp(typeFieldName), typeField->mType};
    method.mBody.Add("return {};", typeField->mName);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void ClassFieldVariantPlugin::ProcessVariantTypeCommon(Class &cls, const std::string &typeFieldName,
                                                       MethodBase &method,
                                                       VariantTypeProcessType processType) {
  auto typeField = cls.GetField(Naming().FieldNameInCpp(typeFieldName));
  if (processType == VariantTypeProcessType::Resetter) {
    method.mBody.Add("if ({} == {}::Invalid) {{", typeField->mName, typeField->mType.mName);
    method.mBody.Indent(1);
    method.mBody.Add("return;");
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  } else if (processType == VariantTypeProcessType::Setter) {
    method.mBody.Add(
        R"R(HOLGEN_FAIL_IF({0} != {1}::Invalid, "{2} field was already initialized (as {{}}), trying to initialize as {{}}!,", {0}, val);)R",
        typeField->mName, typeField->mType.mName, typeFieldName);
    method.mBody.Add("{} = val;", typeField->mName);
  }
  std::vector<ClassField *> matchingFields;
  for (auto &field: cls.mFields) {
    if (!field.mField || field.mField->mType.mName != St::Variant)
      continue;
    if (!field.mField->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_TypeField,
                                            typeFieldName))
      continue;
    matchingFields.push_back(&field);
  }
  bool isFirst = true;
  std::string varNameToCheck = "val";
  if (processType != VariantTypeProcessType::Setter) {
    varNameToCheck = typeField->mName;
  }
  auto matchingClasses = mProject.GetVariantClassesOfEnum(typeField->mType.mName);
  for (auto &[matchingClass, enumEntry]: matchingClasses) {
    auto &projectStruct = *matchingClass->mStruct;
    if (projectStruct.mIsMixin || matchingFields.empty())
      continue;
    auto structVariantAnnotation = projectStruct.GetAnnotation(Annotations::Variant);
    if (isFirst) {
      isFirst = false;
      method.mBody.Add(
          "if ({} == {}::{}) {{", varNameToCheck, typeField->mType.mName,
          structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
    } else {
      method.mBody.Add(
          "}} else if ({} == {}::{}) {{", varNameToCheck, typeField->mType.mName,
          structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
    }
    method.mBody.Indent(1);
    for (auto &field: matchingFields) {
      if (processType == VariantTypeProcessType::Resetter) {
        method.mBody.Add("{}()->~{}();",
                         Naming().VariantGetterNameInCpp(*field->mField, projectStruct),
                         projectStruct.mName);
      } else if (processType == VariantTypeProcessType::Setter) {
        method.mBody.Add("new ({}.data()) {}();", field->mName, projectStruct.mName);
      } else if (processType == VariantTypeProcessType::Mover) {
        method.mBody.Add("*{0}() = std::move(*rhs.{0}());",
                         Naming().VariantGetterNameInCpp(*field->mField, projectStruct));
      } else {
        method.mBody.Add("*{0}() = *rhs.{0}();",
                         Naming().VariantGetterNameInCpp(*field->mField, projectStruct));
      }
    }
    method.mBody.Indent(-1);
  }

  if (!isFirst) {
    method.mBody.Add("}}");
  }

  if (processType == VariantTypeProcessType::Resetter) {
    method.mBody.Add("{0} = {1}({1}::Invalid);", typeField->mName, typeField->mType.mName);
  }
}
} // namespace holgen
