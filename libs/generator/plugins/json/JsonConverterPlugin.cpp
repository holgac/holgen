#include "JsonConverterPlugin.h"
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void JsonConverterPlugin::Run() {
    Validate().JsonConverters();
    auto cls = Class{St::Converter};
    std::set<std::string> processedConverters;
    for (const auto &structDefinition: mProject.mProject.mStructs) {
      for (const auto &fieldDefinition: structDefinition.mFields) {
        auto jsonConvert = fieldDefinition.GetAnnotation(Annotations::JsonConvert);
        if (jsonConvert == nullptr)
          continue;
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        if (processedConverters.contains(jsonConvertUsing->mValue.mName))
          continue;
        processedConverters.insert(jsonConvertUsing->mValue.mName);
        auto field = ClassField{jsonConvertUsing->mValue.mName,
                                Type{"std::function"}, Visibility::Public};

        auto fieldNameInCpp = Naming().FieldNameInCpp(fieldDefinition);
        auto referencedClass = mProject.GetClass(structDefinition.mName);

        field.mType.mFunctionalTemplateParameters.emplace_back(referencedClass->GetField(fieldNameInCpp)->mType);
        field.mType.mFunctionalTemplateParameters.emplace_back(mProject.mProject, jsonConvertFrom->mValue);
        field.mType.mFunctionalTemplateParameters.back().PreventCopying();
        Validate().NewField(cls, field);
        cls.mFields.emplace_back(std::move(field));
      }
    }
    Validate().NewClass(cls);
    mProject.mClasses.emplace_back(std::move(cls));
  }
}
