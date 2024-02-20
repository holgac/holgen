#include <map>
#include <set>
#include "Translator.h"

namespace holgen {

  namespace {
    std::map<std::string, std::string> TypeToCppType = {
        {"s8",    "int8_t"},
        {"s16",   "int16_t"},
        {"s32",   "int32_t"},
        {"s64",   "int64_t"},
        {"u8",    "uint8_t"},
        {"u16",   "uint16_t"},
        {"u32",   "uint32_t"},
        {"u64",   "uint64_t"},
        {"float", "float"},
    };
    std::set<std::string> CppPrimitives = {
        {"int8_t"},
        {"int16_t"},
        {"int32_t"},
        {"int64_t"},
        {"uint8_t"},
        {"uint16_t"},
        {"uint32_t"},
        {"uint64_t"},
        {"float"},
    };
  }

  TranslatedProject Translator::Translate(const Project &project) {
    TranslatedProject translatedProject;
    for (auto &structDefinition: project.mStructs) {
      GenerateClass(translatedProject.mClasses.emplace_back(), structDefinition);
    }
    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }
    // check out rapidjson or something?
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();
    auto capitalizedFieldName = fieldDefinition.mName;

    if (capitalizedFieldName[0] >= 'a' && capitalizedFieldName[0] <= 'z') {
      capitalizedFieldName[0] -= 'a' - 'A';
    }

    generatedField.mName = "m" + capitalizedFieldName;
    auto it = TypeToCppType.find(fieldDefinition.mType);
    if (it == TypeToCppType.end()) {
      throw GeneratorException("Non-primitives not yet supported!");
    }
    generatedField.mType = {it->second, false};

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
      if (!generatedField.mType.IsPrimitive()) {
        arg.mType.mIsConst = true;
        arg.mType.mType = TypeType::Reference;
      }
      arg.mType = generatedField.mType;
      arg.mName = "val";
      setter.mBody.Line() << generatedField.mName << " = val;";
      setter.mType.mName = "void";
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
