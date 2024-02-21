#include <map>
#include <set>
#include <format>
#include "Translator.h"

namespace holgen {

  namespace {
    std::map<std::string, std::string> TypeToCppType = {
        {"s8",      "int8_t"},
        {"s16",     "int16_t"},
        {"s32",     "int32_t"},
        {"s64",     "int64_t"},
        {"u8",      "uint8_t"},
        {"u16",     "uint16_t"},
        {"u32",     "uint32_t"},
        {"u64",     "uint64_t"},
        {"float",   "float"},
        {"double",  "double"},
        {"string",  "std::string"},
        {"vector",  "std::vector"},
        {"map",     "std::map"},
        {"hashmap", "std::unordered_map"},
    };
    std::set<std::string> CppContainers = {
        "std::vector",
    };
    std::set<std::string> CppKeyedContainers = {
        "std::map",
        "std::unordered_map",
    };
    std::set<std::string> CppPrimitives = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
        "float",
        "double",
        "bool",
    };

    struct RapidJsonTypeUsage {
      std::string mValidator;
      std::string mGetter;
    };

    std::map<std::string, RapidJsonTypeUsage> RapidJsonUsage = {
        {"int8_t",      {"IsInt",    "GetInt"}},
        {"int16_t",     {"IsInt",    "GetInt"}},
        {"int32_t",     {"IsInt",    "GetInt"}},
        {"int64_t",     {"IsInt",    "GetInt"}},
        {"uint8_t",     {"IsUint",   "GetUint"}},
        {"uint16_t",    {"IsUint",   "GetUint"}},
        {"uint32_t",    {"IsUint",   "GetUint"}},
        {"uint64_t",    {"IsUint",   "GetUint"}},
        {"float",       {"IsFloat",  "GetFloat"}},
        {"double",      {"IsDouble", "GetDouble"}},
        {"bool",        {"IsBool",   "GetBool"}},
        {"std::string", {"IsString", "GetString"}},
    };

    std::string GetCapitalizedName(const std::string &name) {
      auto capitalizedName = name;

      if (capitalizedName[0] >= 'a' && capitalizedName[0] <= 'z') {
        capitalizedName[0] -= 'a' - 'A';
      }
      return capitalizedName;
    }

    std::string GetFieldName(const FieldDefinition &fieldDefinition) {
      return "m" + GetCapitalizedName(fieldDefinition.mName);
    }

  }

  TranslatedProject Translator::Translate(const ProjectDefinition &project) const {
    TranslatedProject translatedProject;
    for (auto &structDefinition: project.mStructs) {
      GenerateClass(translatedProject.mClasses.emplace_back(), structDefinition);
    }

    GenerateJsonHelper(translatedProject.mClasses.emplace_back());

    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }

    // TODO: skip if noJson decorator exists
    GenerateParseJson(generatedClass, structDefinition);
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();
    auto capitalizedFieldName = GetCapitalizedName(fieldDefinition.mName);

    generatedField.mName = GetFieldName(fieldDefinition);
    ProcessType(generatedField.mType, fieldDefinition.mType);

    {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = "Get" + capitalizedFieldName;
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      // TODO: non-primitives will return const ref
      getter.mType = generatedField.mType;
      getter.mIsConst = true;
      if (!generatedField.mType.IsPrimitive()) {
        getter.mType.mIsConst = true;
        getter.mType.mType = PassByType::Reference;
      }
    }

    // non-const getter for non-primitives only
    if (!generatedField.mType.IsPrimitive()) {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = "Get" + capitalizedFieldName;
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mType = generatedField.mType;
      getter.mIsConst = false;
      if (!generatedField.mType.IsPrimitive()) {
        getter.mType.mIsConst = false;
        getter.mType.mType = PassByType::Reference;
      }
    }

    {
      auto &setter = generatedClass.mMethods.emplace_back();
      setter.mName = "Set" + capitalizedFieldName;
      setter.mIsConst = false;
      auto &arg = setter.mArguments.emplace_back();
      arg.mType = generatedField.mType;
      if (!generatedField.mType.IsPrimitive()) {
        arg.mType.mIsConst = true;
        arg.mType.mType = PassByType::Reference;
      }
      arg.mName = "val";
      setter.mBody.Line() << generatedField.mName << " = val;";
      setter.mType.mName = "void";
    }
  }

  void Translator::GenerateParseJson(Class &generatedClass, const StructDefinition &structDefinition) const {
    auto &func = generatedClass.mMethods.emplace_back();
    func.mName = "ParseJson";
    func.mIsConst = false;
    func.mType.mName = "bool";
    {
      auto &arg = func.mArguments.emplace_back();
      arg.mType.mName = "rapidjson::Value";
      arg.mType.mType = PassByType::Reference;
      arg.mType.mIsConst = true;
      arg.mName = "json";
    }
    func.mBody.Line() << "for(const auto& data: json.GetObject()) {";
    func.mBody.Indent(1);
    func.mBody.Line() << "const auto& name = data.name.GetString();";
    // func.mBody.Line() << "const auto& value = data.value;";
    bool isFirst = true;
    for (const auto &fieldDefinition: structDefinition.mFields) {
      if (isFirst) {
        func.mBody.Line() << "if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
        isFirst = false;
      } else {
        func.mBody.Line() << "} else if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
      }
      func.mBody.Indent(1);
      func.mBody.Line() << "auto res = JsonHelper::Parse(" << GetFieldName(fieldDefinition) << ", data.value);";
      func.mBody.Line() << "if (!res)";
      func.mBody.Indent(1);
      func.mBody.Line() << "return false;";
      func.mBody.Indent(-1); // if !res
      func.mBody.Indent(-1); // if name == fieldName
    }

    if (!structDefinition.mFields.empty())
      func.mBody.Line() << "}";
    func.mBody.Indent(-1);
    func.mBody.Line() << "}"; // range based for on json.GetObject()
    func.mBody.Line() << "return true;";
  }

  // TODO: move this to a separate file
  void Translator::GenerateJsonHelper(Class &generatedClass) const {
    generatedClass.mName = "JsonHelper";
    auto &baseParse = generatedClass.mMethods.emplace_back();
    baseParse.mName = "Parse";
    baseParse.mIsConst = false;
    baseParse.mIsStatic = true;
    auto& baseTemplateArg = baseParse.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";
    baseParse.mType.mName = "bool";

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

    baseParse.mBody.Line() << "return out.ParseJson(json);";

    for (const auto[type, usage]: RapidJsonUsage) {
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

      parse.mBody.Line() << "if (!json." << usage.mValidator << "())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "out = json." << usage.mGetter << "();";
      parse.mBody.Line() << "return true;";
    }

    for (const auto &container: CppContainers) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = "Parse";
      parse.mIsConst = false;
      parse.mIsStatic = true;
      auto& templateArg = parse.mTemplateParameters.emplace_back();
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

      parse.mBody.Line() << "if (!json.IsArray())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);

      parse.mBody.Line() << "for (const auto& data: json.GetArray()) {";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "auto res = Parse(out.emplace_back(), data);";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";

    }

    for (const auto &container: CppKeyedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = "Parse";
      parse.mIsConst = false;
      parse.mIsStatic = true;
      auto& keyTemplateArg = parse.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto& valueTemplateArg = parse.mTemplateParameters.emplace_back();
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

      parse.mBody.Line() << "if (!json.IsObject())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if (!json.IsObject())

      parse.mBody.Line() << "for (const auto& data: json.GetObject()) {";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "K key;";
      parse.mBody.Line() << "Parse(key, data.name);";
      parse.mBody.Line() << "auto [it, res] = out.try_emplace(key, V());";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Line() << "res = Parse(it->second, data.value);";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";
    }
  }

  void Translator::ProcessType(Type &type, const TypeDefinition &typeDefinition) const {
    auto it = TypeToCppType.find(typeDefinition.mName);
    if (it != TypeToCppType.end()) {
      type.mName = it->second;
    } else {
      type.mName = typeDefinition.mName;
    }

    for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
      ProcessType(type.mTemplateParameters.emplace_back(), templateParameter);
    }
  }

  bool Type::IsPrimitive() const {
    return CppPrimitives.contains(mName);
  }

  void CodeBlock::Add(const CodeBlock &cb) {
    mContents.insert(mContents.end(), cb.mContents.begin(), cb.mContents.end());
    mLines.insert(mLines.end(), cb.mLines.begin(), cb.mLines.end());
    mIndentations.insert(mIndentations.end(), cb.mIndentations.begin(), cb.mIndentations.end());
  }
}
