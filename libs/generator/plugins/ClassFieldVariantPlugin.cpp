#include "ClassFieldVariantPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "core/Exception.h"

namespace holgen {
  void ClassFieldVariantPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      ProcessStructDefinition(cls, *cls.mStruct);
    }
  }

  void ClassFieldVariantPlugin::ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition) {
    for (auto &mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
    }
    for (auto &fieldDefinition: structDefinition.mFields) {
      if (fieldDefinition.mType.mName != St::Variant)
        continue;
      ProcessVariantField(cls, fieldDefinition);
    }
  }

  void ClassFieldVariantPlugin::ProcessVariantField(Class &cls, const FieldDefinition &fieldDefinition) {
    // TODO: validate variant annotations
    auto variantAnnotation = fieldDefinition.GetAnnotation(Annotations::Variant);
    THROW_IF(!variantAnnotation, "variant fields should have a variant annotation!");
    auto type = variantAnnotation->GetAttribute(Annotations::Variant_TypeField);
    auto enumName = variantAnnotation->GetAttribute(Annotations::Variant_Enum);
    THROW_IF(!type || !enumName, "Insufficient variant annotation");
    std::stringstream arraySizeSpecifier;
    auto dataField = ClassField{Naming().FieldNameInCpp(fieldDefinition), Type{"std::array"}};
    size_t matchingStructCount = 0;
    // TODO: validate that there are structs matching the given enum
    for (auto &projectStruct: mProject.mProject.mStructs) {
      if (projectStruct.mIsMixin)
        continue;
      // TODO: don't allow variant structs to be inherited from
      auto structVariantAnnotation = projectStruct.GetAnnotation(Annotations::Variant);
      if (!structVariantAnnotation ||
          structVariantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName != enumName->mValue.mName)
        continue;
      if (matchingStructCount != 0)
        arraySizeSpecifier << ", ";
      ++matchingStructCount;
      cls.mHeaderIncludes.AddLocalHeader(projectStruct.mName + ".h");
      arraySizeSpecifier << "sizeof(" << projectStruct.mName << ")";
      auto entryStr = std::format("{}::{}", enumName->mValue.mName,
                                  structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
      for (int i=0; i<2; ++i)
      {
        auto constness = i == 0 ? Constness::Const : Constness::NotConst;
        auto method = ClassMethod{Naming().VariantGetterNameInCpp(fieldDefinition, projectStruct),
                                  Type{projectStruct.mName, PassByType::Pointer, constness}, Visibility::Public,
                                  constness};
        method.mBody.Add(
            R"R(HOLGEN_FAIL_IF({} != {}, "Attempting to get {}.{} as {} while in reality its type is {{}}!", {});)R",
            Naming().FieldNameInCpp(type->mValue.mName), entryStr, cls.mName, fieldDefinition.mName,
            projectStruct.mName, Naming().FieldNameInCpp(type->mValue.mName));
        method.mBody.Add("return reinterpret_cast<{}>({}.data());", method.mReturnType.ToString(), dataField.mName);
        Validate().NewMethod(cls, method);
        cls.mMethods.push_back(std::move(method));
      }
      {
        auto method = ClassMethod{
            std::format("Initialize{}As{}", St::Capitalize(fieldDefinition.mName),
                        St::Capitalize(
                            structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName)),
            Type{"void"}, Visibility::Public, Constness::NotConst};
        method.mBody.Add(
            R"R(HOLGEN_FAIL_IF({0} != {1}::Invalid, "{2} field was already initialized as {{}}, trying to initialize as {{}}!,", {0}, {3});)R",
            Naming().FieldNameInCpp(type->mValue.mName), enumName->mValue.mName, fieldDefinition.mName, entryStr);
        method.mBody.Add("{} = {};", Naming().FieldNameInCpp(type->mValue.mName), entryStr);
        method.mBody.Add("new ({}.data()) {}();", dataField.mName, projectStruct.mName);
        Validate().NewMethod(cls, method);
        cls.mMethods.push_back(std::move(method));
      }
    }

    auto typeGetter = ClassMethod{std::format("Get{}Type", St::Capitalize(fieldDefinition.mName)),
                                  Type{enumName->mValue.mName}};
    typeGetter.mBody.Add("return {};", Naming().FieldNameInCpp(type->mValue.mName));
    Validate().NewMethod(cls, typeGetter);
    cls.mMethods.emplace_back(std::move(typeGetter));

    if (cls.GetField(Naming().FieldNameInCpp(type->mValue.mName)) == nullptr) {
      auto typeField = ClassField{
          Naming().FieldNameInCpp(type->mValue.mName),
          Type{enumName->mValue.mName}};
      Validate().NewField(cls, typeField);
      cls.mFields.emplace_back(std::move(typeField));
    }
    THROW_IF(matchingStructCount == 0, "Variant field with no matching struct");
    dataField.mType.mTemplateParameters.emplace_back("uint8_t");
    if (matchingStructCount > 1)
      dataField.mType.mTemplateParameters.emplace_back("std::max(" + arraySizeSpecifier.str() + ")");
    else
      dataField.mType.mTemplateParameters.emplace_back(arraySizeSpecifier.str());
    dataField.mField = &fieldDefinition;
    Validate().NewField(cls, dataField);
    cls.mFields.emplace_back(std::move(dataField));

  }
}
