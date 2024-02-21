#include <map>
#include <set>
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
        {"double",   "double"},
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

    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }

    // TODO: skip if noJson decorator exists
    // GenerateParseJson(generatedClass, structDefinition);
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
        getter.mType.mType = TypeType::Reference;
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
        getter.mType.mType = TypeType::Reference;
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
        arg.mType.mType = TypeType::Reference;
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
    {
      auto &arg = func.mArguments.emplace_back();
      arg.mType.mName = "rapidjson::Value";
      arg.mType.mType = TypeType::Reference;
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
      func.mBody.Line() << "JsonHelper::Parse(" << GetFieldName(fieldDefinition) << ", data.value);";
      func.mBody.Indent(-1); // if name == fieldName
    }

    if (!structDefinition.mFields.empty())
      func.mBody.Line() << "}";
    func.mBody.Indent(-1);
    func.mBody.Line() << "}"; // range based for on json.GetObject()
  }

  void Translator::GenerateJsonHelper(Class &generatedClass) const {
    generatedClass.mName = "JsonHelper";

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
