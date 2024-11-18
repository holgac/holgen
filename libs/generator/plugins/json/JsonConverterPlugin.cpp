#include "JsonConverterPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/TypeInfo.h"

namespace holgen {
void JsonConverterPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }
  Validate().JsonConverters();
  auto cls = Class{St::Converter, mSettings.mNamespace};
  cls.mFields.emplace_back(Naming().FieldNameInCpp(St::Converter_BypassField), Type{"bool"},
                           Visibility::Public, Staticness::NotStatic, "false");
  std::set<std::string> processedConverters;
  std::map<std::string, ClassField> converterFields;
  for (const auto &targetCls: mProject.mClasses) {
    for (const auto &targetField: targetCls.mFields) {
      if (!targetField.mField || !targetField.mField->GetAnnotation(Annotations::JsonConvert))
        continue;
      auto jsonConvert = targetField.mField->GetAnnotation(Annotations::JsonConvert);
      auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
      auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
      auto jsonConvertElem = jsonConvert->GetAttribute(Annotations::JsonConvert_Elem);
      if (processedConverters.contains(jsonConvertUsing->mValue.mName))
        continue;
      processedConverters.insert(jsonConvertUsing->mValue.mName);
      auto field =
          ClassField{jsonConvertUsing->mValue.mName, Type{"std::function"}, Visibility::Public};

      if (jsonConvertElem)
        field.mType.mFunctionalTemplateParameters.emplace_back(
            targetField.mType.mTemplateParameters.back());
      else
        field.mType.mFunctionalTemplateParameters.emplace_back(targetField.mType);
      field.mType.mFunctionalTemplateParameters.emplace_back(
          mProject, jsonConvert->mDefinitionSource, jsonConvertFrom->mValue);
      field.mType.mFunctionalTemplateParameters.back().PreventCopying();
      Validate().NewField(cls, field);
      converterFields.emplace(field.mName, field);
    }
  }
  for (auto &[fieldName, field]: converterFields) {
    cls.mFields.push_back(std::move(field));
  }
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}
} // namespace holgen
