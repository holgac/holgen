#include "GeneratorJson.h"
#include <vector>
#include "TypeInfo.h"
#include "core/Decorators.h"
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
  }

  GeneratorJson::GeneratorJson(
      const ProjectDefinition &projectDefinition,
      TranslatedProject &translatedProject
  ) : mProjectDefinition(projectDefinition), mTranslatedProject(translatedProject) {
    for (size_t i = 0; i < mTranslatedProject.mClasses.size(); ++i) {
      const auto &cls = mTranslatedProject.mClasses[i];
      mClasses.emplace(cls.mName, i);
    }
  }

  void GeneratorJson::EnrichClasses() {
    for (auto &cls: mTranslatedProject.mClasses) {
      const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
      if (structDefinition.GetDecorator(Decorators::NoJson))
        continue;
      if (structDefinition.GetDecorator(Decorators::DataManager))
        GenerateParseFiles(cls);
      else
        // TODO: currently we iterate over the json obj when deserializing, but this wouldn't work with
        // dependencies. For DataManager we should have something custom so that a single file can define
        // everything too. But for now ParseFiles is good enough.
        GenerateParseJson(cls);
    }
  }

  void GeneratorJson::GenerateParseFiles(Class &cls) {
    const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
    auto &parseFunc = cls.mMethods.emplace_back();
    parseFunc.mName = "ParseFiles";
    parseFunc.mIsConst = false;
    parseFunc.mType.mName = "bool";
    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = "std::string";
      arg.mType.mType = PassByType::Reference;
      arg.mType.mIsConst = true;
      arg.mName = "rootPath";
    }
    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = ConverterName;
      arg.mType.mType = PassByType::Reference;
      arg.mType.mIsConst = true;
      arg.mName = "converterArg";
    }
    parseFunc.mBody.Add("auto converter = converterArg;");
    for (const auto &fieldDefinition: structDefinition.mFields) {
      auto containerDecorator = fieldDefinition.GetDecorator(Decorators::Container);
      if (containerDecorator == nullptr)
        continue;
      for (const auto &decoratorDefinition: fieldDefinition.mDecorators) {
        if (decoratorDefinition.mName != Decorators::Index)
          continue;
        auto &underlyingStruct = *mProjectDefinition.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
        auto indexedOnField = underlyingStruct.GetField(
            decoratorDefinition.GetAttribute(Decorators::Index_On)->mValue.mName);
        auto forConverter = decoratorDefinition.GetAttribute(Decorators::Index_ForConverter);
        if (forConverter == nullptr)
          continue;
        parseFunc.mBody.Add("if (converter.{} == nullptr) {{", forConverter->mValue.mName);
        parseFunc.mBody.Indent(1);

        Type fromType;
        TypeInfo::Get().ConvertToType(fromType, indexedOnField->mType);
        if (!TypeInfo::Get().CppPrimitives.contains(fromType.mName)) {
          // This is done so many times, maybe Type::AdjustForFunctionArgument?
          fromType.mIsConst = true;
          fromType.mType = PassByType::Reference;
        }
        auto idField = underlyingStruct.GetIdField();
        Type toType;
        TypeInfo::Get().ConvertToType(toType, idField->mType);
        parseFunc.mBody.Add("converter.{} = [this]({} key) -> {} {{", forConverter->mValue.mName, fromType.ToString(),
                            toType.ToString());
        parseFunc.mBody.Indent(1);

        auto &elementName = *containerDecorator->GetAttribute(Decorators::Container_ElemName);
        parseFunc.mBody.Add("auto elem = {}(key);", St::GetIndexGetterName(elementName.mValue.mName, indexedOnField->mName));
        parseFunc.mBody.Add("return elem->{}();", St::GetGetterMethodName(idField->mName));

        parseFunc.mBody.Indent(-1);
        parseFunc.mBody.Add("}};"); // converter =

        parseFunc.mBody.Indent(-1);
        parseFunc.mBody.Add("}}"); // if (converter == nullptr)

      }
    }

    parseFunc.mBody.Add("std::map<std::string, std::vector<std::filesystem::path>> filesByName;");
    parseFunc.mBody.Add("std::queue<std::filesystem::path> pathsQueue;");
    parseFunc.mBody.Add("pathsQueue.push(std::filesystem::path(rootPath));");
    parseFunc.mBody.Add("while(!pathsQueue.empty()) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("auto& curPath = pathsQueue.front();");
    parseFunc.mBody.Add("for (auto &entry: std::filesystem::directory_iterator(curPath)) {{");
    parseFunc.mBody.Indent(1);

    parseFunc.mBody.Add("if (std::filesystem::is_directory(entry)) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("pathsQueue.push(entry.path());");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}} else if (std::filesystem::is_regular_file(entry)) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("std::string filename = entry.path().filename();");
    parseFunc.mBody.Add("auto dotPosition = filename.rfind('.');");
    parseFunc.mBody.Add("if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == \"json\") {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("filesByName[filename.substr(0, dotPosition)].push_back(entry.path());");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // if (json)

    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // if (regular file)

    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // for entry : curPath
    parseFunc.mBody.Add("pathsQueue.pop();");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // while(!paths.empty())

    bool isFirst = true;
    for (const auto &structToProcess : mTranslatedProject.mDependencyGraph.GetProcessOrder()) {
      for (const auto &fieldDefinition: structDefinition.mFields) {
        if (!fieldDefinition.GetDecorator(Decorators::Container))
          continue;
        auto &templateParameter = fieldDefinition.mType.mTemplateParameters[0];
        if (templateParameter.mName == structToProcess) {
          {
            auto line = parseFunc.mBody.Line();
            if (isFirst) {
              line << "auto it";
              isFirst = false;
            } else {
              line << "it";
            }
            line << " = filesByName.find(\"" << fieldDefinition.mName << "\");";
          }
          parseFunc.mBody.Add("if (it != filesByName.end()) {{");
          parseFunc.mBody.Indent(1);

          parseFunc.mBody.Add("for (const auto& filePath: it->second) {{");
          parseFunc.mBody.Indent(1);
          parseFunc.mBody.Add("std::ifstream fin(filePath, std::ios_base::binary);");
          parseFunc.mBody.Add("fin.seekg(0, std::ios_base::end);");
          parseFunc.mBody.Add("std::string contents(fin.tellg(), 0);");
          parseFunc.mBody.Add("fin.seekg(0, std::ios_base::beg);");
          parseFunc.mBody.Add("fin.read(contents.data(), contents.size());");
          parseFunc.mBody.Add("rapidjson::Document doc;");
          parseFunc.mBody.Add("doc.Parse(contents.c_str());");
          parseFunc.mBody.Add("if (!doc.IsArray()) {{");
          parseFunc.mBody.Indent(1);
          // TODO: should use a ON_FAILURE(msg, ...) macro (and CHECK(cond, msg, ...)) and define ifndef.
          // Can make it a GeneratorSettings parameter to include a special user file at each header,
          // and also have own header that every header includes and #define's (ifndef) such macros
          parseFunc.mBody.Add("return false;");
          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // if (!doc.IsArray())

          parseFunc.mBody.Add("for (auto& jsonElem: doc.GetArray()) {{"); // if (!doc.IsArray())
          parseFunc.mBody.Indent(1);
          Type type;
          TypeInfo::Get().ConvertToType(type, templateParameter);
          parseFunc.mBody.Add("{} elem;", type.ToString()); // if (!doc.IsArray())
          parseFunc.mBody.Add("elem.ParseJson(jsonElem, converter);", type.ToString()); // if (!doc.IsArray())
          auto elemName = fieldDefinition.GetDecorator(Decorators::Container)->GetAttribute(
              Decorators::Container_ElemName);
          parseFunc.mBody.Add("{}(std::move(elem));", St::GetAdderMethodName(elemName->mValue.mName));
          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // for (jsonElem: doc.GetArray())

          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // for(path: filesByName[field])

          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // if (it != filesByName.end())
        }
      }
    }
    parseFunc.mBody.Add("return true;");
  }

  void GeneratorJson::GenerateParseJson(Class &cls) {
    const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
    auto &parseFunc = cls.mMethods.emplace_back();
    parseFunc.mName = "ParseJson";
    parseFunc.mIsConst = false;
    parseFunc.mType.mName = "bool";
    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = "rapidjson::Value";
      arg.mType.mType = PassByType::Reference;
      arg.mType.mIsConst = true;
      arg.mName = "json";
    }

    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = ConverterName;
      arg.mType.mType = PassByType::Reference;
      arg.mType.mIsConst = true;
      arg.mName = "converter";
    }
    parseFunc.mBody.Line() << "for(const auto& data: json.GetObject()) {";
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Line() << "const auto& name = data.name.GetString();";
    // parseFunc.mBody.Line() << "const auto& value = data.value;";
    bool isFirst = true;
    for (const auto &fieldDefinition: structDefinition.mFields) {
      if (fieldDefinition.GetDecorator(Decorators::NoJson))
        continue;
      if (isFirst) {
        parseFunc.mBody.Line() << "if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
        isFirst = false;
      } else {
        parseFunc.mBody.Line() << "} else if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
      }
      GenerateParseJsonForField(cls, parseFunc, structDefinition, fieldDefinition);
      parseFunc.mBody.Indent(-1); // if name == fieldName
    }

    if (!structDefinition.mFields.empty())
      parseFunc.mBody.Line() << "}";
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Line() << "}"; // range based for on json.GetObject()
    parseFunc.mBody.Line() << "return true;";
  }

  void GeneratorJson::GenerateParseJsonForField(
      Class &cls,
      ClassMethod &parseFunc,
      const StructDefinition &structDefinition,
      const FieldDefinition &fieldDefinition
  ) {
    parseFunc.mBody.Indent(1);
    auto fieldClassIt = mClasses.find(fieldDefinition.mType.mName);
    if (fieldClassIt == mClasses.end()) {
      auto jsonConvert = fieldDefinition.GetDecorator(Decorators::JsonConvert);
      if (jsonConvert != nullptr) {
        auto jsonConvertFrom = jsonConvert->GetAttribute(Decorators::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Decorators::JsonConvert_Using);
        Type type;
        TypeInfo::Get().ConvertToType(type, jsonConvertFrom->mValue);
        parseFunc.mBody.Line() << type.ToString() << " temp;";
        parseFunc.mBody.Line() << "auto res = JsonHelper::Parse(temp, data.value, converter);";
        parseFunc.mBody.Line() << "if (!res)";
        parseFunc.mBody.Indent(1);
        parseFunc.mBody.Line() << "return false;";
        parseFunc.mBody.Indent(-1); // if !res
        Type fieldType;
        TypeInfo::Get().ConvertToType(fieldType, fieldDefinition.mType);
        if (TypeInfo::Get().CppPrimitives.contains(fieldType.mName))
          parseFunc.mBody.Add("{} = converter.{}(temp);", St::GetFieldNameInCpp(fieldDefinition.mName),
                              jsonConvertUsing->mValue.mName);
        else
          parseFunc.mBody.Add("{} = std::move(converter.{}(temp));", St::GetFieldNameInCpp(fieldDefinition.mName),
                              jsonConvertUsing->mValue.mName);
      } else {
        parseFunc.mBody.Line() << "auto res = JsonHelper::Parse(" << St::GetFieldNameInCpp(fieldDefinition.mName)
                               << ", data.value, converter);";
        parseFunc.mBody.Line() << "if (!res)";
        parseFunc.mBody.Indent(1);
        parseFunc.mBody.Line() << "return false;";
        parseFunc.mBody.Indent(-1); // if !res
      }
    } else {
      parseFunc.mBody.Line() << "auto res = " << St::GetFieldNameInCpp(fieldDefinition.mName)
                             << ".ParseJson(data.value, converter);";

      parseFunc.mBody.Line() << "if (!res)";
      parseFunc.mBody.Indent(1);
      parseFunc.mBody.Line() << "return false;";
      parseFunc.mBody.Indent(-1); // if !res
    }
  }

  void GeneratorJson::GenerateHelpers() {
    GenerateJsonHelper(mTranslatedProject.mClasses.emplace_back());
    GenerateConverter(mTranslatedProject.mClasses.emplace_back());
  }

  void GeneratorJson::GenerateJsonHelper(Class &generatedClass) {
    generatedClass.mName = "JsonHelper";
    auto &baseParse = generatedClass.mMethods.emplace_back();
    baseParse.mName = "Parse";
    baseParse.mIsConst = false;
    baseParse.mIsStatic = true;
    baseParse.mType.mName = "bool";
    auto &baseTemplateArg = baseParse.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    {
      auto &out = baseParse.mArguments.emplace_back();
      out.mName = "out";
      out.mType.mName = "T";
      out.mType.mType = PassByType::Reference;
    }

    {
      auto &json = baseParse.mArguments.emplace_back();
      json.mName = "json";
      json.mType.mName = "rapidjson::Value";
      json.mType.mIsConst = true;
      json.mType.mType = PassByType::Reference;
    }

    {
      auto &converter = baseParse.mArguments.emplace_back();
      converter.mName = "converter";
      converter.mType.mName = ConverterName;
      converter.mType.mIsConst = true;
      converter.mType.mType = PassByType::Reference;
    }

    baseParse.mBody.Line() << "return out.ParseJson(json, converter);";

    for (const auto &[type, usage]: RapidJsonUsage) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = "Parse";
      parse.mIsConst = false;
      parse.mIsStatic = true;
      parse.mIsTemplateSpecialization = true;
      parse.mType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = type;
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mIsConst = true;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mIsConst = true;
        converter.mType.mType = PassByType::Reference;
      }

      parse.mBody.Line() << "if (!json." << usage.mValidator << "())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "out = json." << usage.mGetter << "();";
      parse.mBody.Line() << "return true;";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = "Parse";
      parse.mIsConst = false;
      parse.mIsStatic = true;
      auto &templateArg = parse.mTemplateParameters.emplace_back();
      templateArg.mType = "typename";
      templateArg.mName = "T";
      parse.mType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = container;
        auto &outTemplate = out.mType.mTemplateParameters.emplace_back();
        outTemplate.mName = "T";
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mIsConst = true;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mIsConst = true;
        converter.mType.mType = PassByType::Reference;
      }

      parse.mBody.Line() << "if (!json.IsArray())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);

      parse.mBody.Line() << "for (const auto& data: json.GetArray()) {";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "auto res = Parse(out.emplace_back(), data, converter);";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";

    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = "Parse";
      parse.mIsConst = false;
      parse.mIsStatic = true;
      auto &keyTemplateArg = parse.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = parse.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";
      parse.mType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = container;
        auto &keyTemplate = out.mType.mTemplateParameters.emplace_back();
        keyTemplate.mName = "K";
        auto &valueTemplate = out.mType.mTemplateParameters.emplace_back();
        valueTemplate.mName = "V";
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mIsConst = true;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mIsConst = true;
        converter.mType.mType = PassByType::Reference;
      }

      parse.mBody.Line() << "if (!json.IsObject())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if (!json.IsObject())

      parse.mBody.Line() << "for (const auto& data: json.GetObject()) {";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "K key;";
      parse.mBody.Line() << "Parse(key, data.name, converter);";
      parse.mBody.Line() << "auto [it, res] = out.try_emplace(key, V());";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Line() << "res = Parse(it->second, data.value, converter);";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";
    }
  }

  void GeneratorJson::GenerateConverter(Class &cls) {
    cls.mName = "Converter";
    std::set<std::string> processedConverters;
    for (const auto &structDefinition: mProjectDefinition.mStructs) {
      for (const auto &fieldDefinition: structDefinition.mFields) {
        auto jsonConvert = fieldDefinition.GetDecorator(Decorators::JsonConvert);
        if (jsonConvert == nullptr)
          continue;
        auto jsonConvertFrom = jsonConvert->GetAttribute(Decorators::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Decorators::JsonConvert_Using);
        if (processedConverters.contains(jsonConvertUsing->mValue.mName))
          continue;
        processedConverters.insert(jsonConvertUsing->mValue.mName);
        auto &func = cls.mFields.emplace_back();
        func.mVisibility = Visibility::Public;
        func.mName = jsonConvertUsing->mValue.mName;
        func.mType.mName = "std::function";

        auto &convertFromArg = func.mType.mFunctionalTemplateParameters.emplace_back();
        TypeInfo::Get().ConvertToType(convertFromArg, fieldDefinition.mType);

        auto &retVal = func.mType.mFunctionalTemplateParameters.emplace_back();
        TypeInfo::Get().ConvertToType(retVal, jsonConvertFrom->mValue);
        if (!TypeInfo::Get().CppPrimitives.contains(retVal.mName)) {
          retVal.mIsConst = true;
          retVal.mType = PassByType::Reference;
        }

      }
    }

  }

}
