#include "JsonConverterPlugin.h"
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "../../Naming.h"

namespace holgen {
  namespace {
    std::string ConverterName = "Converter";
  }

  void JsonConverterPlugin::Run() {
    auto &generatedClass = mProject.mClasses.emplace_back(ConverterName);
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
        auto &func = generatedClass.mFields.emplace_back(
            jsonConvertUsing->mValue.mName,
            Type{"std::function"},
            Visibility::Public
        );


        auto fieldNameInCpp = Naming(mProject).FieldNameInCpp(fieldDefinition);
        auto referencedClass = mProject.GetClass(structDefinition.mName);

        func.mType.mFunctionalTemplateParameters.emplace_back(referencedClass->GetField(fieldNameInCpp)->mType);

        func.mType.mFunctionalTemplateParameters.emplace_back(mProject.mProject, jsonConvertFrom->mValue);
        func.mType.mFunctionalTemplateParameters.back().PreventCopying();
      }
    }
  }
}
