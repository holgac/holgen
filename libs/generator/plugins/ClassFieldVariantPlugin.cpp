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
    auto typeField = ClassField{
        Naming().FieldNameInCpp(type->mValue.mName),
        Type{enumName->mValue.mName}};

    std::stringstream arraySizeSpecifier;
    arraySizeSpecifier << "std::max(";
    auto dataField = ClassField{Naming().FieldNameInCpp(fieldDefinition), Type{"std::array"}};
    bool isFirst = true;
    // TODO: validate that there are structs matching the given enum
    for (auto &projectStruct: mProject.mProject.mStructs) {
      if (projectStruct.mIsMixin)
        continue;
      // TODO: don't allow variant structs to be inherited from
      auto structVariantAnnotation = projectStruct.GetAnnotation(Annotations::Variant);
      if (!structVariantAnnotation ||
          structVariantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName != enumName->mValue.mName)
        continue;
      if (isFirst)
        isFirst = false;
      else
        arraySizeSpecifier << ", ";
      cls.mHeaderIncludes.AddLocalHeader(projectStruct.mName + ".h");
      arraySizeSpecifier << "sizeof(" << projectStruct.mName << ")";
      auto entryStr = std::format("{}::{}", enumName->mValue.mName,
                                  structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
      {
        // TODO: const counterpart?
        auto method = ClassMethod{Naming().VariantGetterNameInCpp(fieldDefinition, projectStruct),
                                  Type{projectStruct.mName, PassByType::Pointer}, Visibility::Public,
                                  Constness::NotConst};
        method.mBody.Add(
            R"R(HOLGEN_FAIL_IF({} != {}, "Attempting to get {}.{} as {} while in reality its type is {{}}!", {});)R",
            typeField.mName, entryStr, cls.mName, fieldDefinition.mName, projectStruct.mName, typeField.mName);
        method.mBody.Add("return reinterpret_cast<{}*>({}.data());", projectStruct.mName, dataField.mName);
        Validate().NewMethod(cls, method);
        cls.mMethods.push_back(std::move(method));
      }
      {
        auto method = ClassMethod{
            std::format("Initialize{}As{}", St::Capitalize(fieldDefinition.mName),
                        structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName),
            Type{"void"}, Visibility::Public, Constness::NotConst};
        method.mBody.Add(
            R"R(HOLGEN_FAIL_IF({0} != {1}::Invalid, "{2} field was already initialized as {{}}, trying to initialize as {{}}!,", {0}, {3});)R",
            typeField.mName, enumName->mValue.mName, fieldDefinition.mName, entryStr);
        method.mBody.Add("{} = {};", typeField.mName, entryStr);
        method.mBody.Add("new ({}.data()) {}();", dataField.mName, projectStruct.mName);
        Validate().NewMethod(cls, method);
        cls.mMethods.push_back(std::move(method));
      }
    }
    Validate().NewField(cls, typeField);
    cls.mFields.emplace_back(std::move(typeField));
    THROW_IF(isFirst, "Variant field with no matching struct");
    arraySizeSpecifier << ")";
    dataField.mType.mTemplateParameters.emplace_back("uint8_t");
    dataField.mType.mTemplateParameters.emplace_back(arraySizeSpecifier.str());
    dataField.mField = &fieldDefinition;
    Validate().NewField(cls, dataField);
    cls.mFields.emplace_back(std::move(dataField));
  }
}
