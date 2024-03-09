#include "JsonConverterPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  namespace {
    struct RapidJsonTypeUsage {
      std::string mValidator;
      std::string mGetter;
    };

    std::map<std::string, RapidJsonTypeUsage> RapidJsonUsage = {
        {"int8_t",      {"IsInt",    "GetInt"}},
        {"int16_t",     {"IsInt",    "GetInt"}},
        {"int32_t",     {"IsInt",    "GetInt"}},
        {"int64_t",     {"IsInt64",  "GetInt64"}},
        {"uint8_t",     {"IsUint",   "GetUint"}},
        {"uint16_t",    {"IsUint",   "GetUint"}},
        {"uint32_t",    {"IsUint",   "GetUint"}},
        {"uint64_t",    {"IsUint64", "GetUint64"}},
        {"float",       {"IsFloat",  "GetFloat"}},
        {"double",      {"IsDouble", "GetDouble"}},
        {"bool",        {"IsBool",   "GetBool"}},
        {"std::string", {"IsString", "GetString"}},
    };

    std::string ConverterName = "Converter";
    std::string ParseJson = "ParseJson";
  }

  void JsonConverterPlugin::GenerateHelpers() {
    GenerateJsonConverter(mProject.mClasses.emplace_back());
  }

  void JsonConverterPlugin::GenerateJsonConverter(Class &generatedClass) {
    generatedClass.mName = ConverterName;
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
        auto &func = generatedClass.mFields.emplace_back();
        func.mVisibility = Visibility::Public;
        func.mName = jsonConvertUsing->mValue.mName;
        func.mType.mName = "std::function";

        auto &convertFromArg = func.mType.mFunctionalTemplateParameters.emplace_back();

        auto fieldNameInCpp = St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref");
        auto referencedClass = mProject.GetClass(structDefinition.mName);

        convertFromArg = referencedClass->GetField(fieldNameInCpp)->mType;
        // TypeInfo::Get().ConvertToType(convertFromArg, fieldDefinition.mType);

        auto &retVal = func.mType.mFunctionalTemplateParameters.emplace_back();
        TypeInfo::Get().ConvertToType(retVal, jsonConvertFrom->mValue);
        if (!TypeInfo::Get().CppPrimitives.contains(retVal.mName)) {
          retVal.mConstness = Constness::Const;
          retVal.mType = PassByType::Reference;
        }
      }
    }
  }
}
