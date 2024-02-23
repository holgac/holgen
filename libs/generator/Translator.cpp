#include "Translator.h"
#include <map>
#include "core/Exception.h"
#include "GeneratorJson.h"
#include "TypeInfo.h"
#include "GeneratorLua.h"

namespace holgen {

  namespace {

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
    std::map<std::string, size_t> classMap;
    for (auto &structDefinition: project.mStructs) {
      auto[it, res] = classMap.try_emplace(structDefinition.mName, translatedProject.mClasses.size());
      THROW_IF(!res, "Duplicate class: \"{}\"", structDefinition.mName)
      GenerateClass(translatedProject.mClasses.emplace_back(), structDefinition);
    }

    // Enrichment should not create new classes or add new fields.
    GeneratorJson generatorJson(project, translatedProject);
    generatorJson.EnrichClasses();
    GeneratorLua generatorLua(project, translatedProject);
    generatorLua.EnrichClasses();

    // After all integrations processed all real structs, create helpers
    generatorJson.GenerateHelpers();
    generatorLua.GenerateHelpers();

    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();
    auto capitalizedFieldName = GetCapitalizedName(fieldDefinition.mName);

    generatedField.mName = GetFieldName(fieldDefinition);
    TypeInfo::Get().ConvertToType(generatedField.mType, fieldDefinition.mType);
    bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);

    {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = "Get" + capitalizedFieldName;
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mType = generatedField.mType;
      getter.mIsConst = true;
      if (!isPrimitive) {
        getter.mType.mIsConst = true;
        getter.mType.mType = PassByType::Reference;
      }
    }

    // non-const getter for non-primitives only
    if (!isPrimitive) {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = "Get" + capitalizedFieldName;
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mType = generatedField.mType;
      getter.mIsConst = false;
      getter.mType.mType = PassByType::Reference;
    }

    {
      auto &setter = generatedClass.mMethods.emplace_back();
      setter.mName = "Set" + capitalizedFieldName;
      setter.mIsConst = false;
      auto &arg = setter.mArguments.emplace_back();
      arg.mType = generatedField.mType;
      if (!isPrimitive) {
        arg.mType.mIsConst = true;
        arg.mType.mType = PassByType::Reference;
      }
      arg.mName = "val";
      setter.mBody.Line() << generatedField.mName << " = val;";
      setter.mType.mName = "void";
    }
  }

  ClassField *Class::GetField(const std::string &name) {
    for(auto& field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }
}
