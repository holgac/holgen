#include "Translator.h"
#include <map>
#include "core/St.h"
#include "core/Exception.h"
#include "generator/translator_plugins/GeneratorJson.h"
#include "generator/translator_plugins/GeneratorLua.h"
#include "generator/translator_plugins/GeneratorGlobalPointer.h"
#include "generator/translator_plugins/GeneratorFilesystemHelper.h"
#include "TypeInfo.h"
#include "core/Annotations.h"
#include "parser/Validator.h"
#include "generator/translator_plugins/ClassPlugin.h"
#include "generator/translator_plugins/ClassFieldPlugin.h"
#include "generator/translator_plugins/ClassFieldGetterPlugin.h"
#include "generator/translator_plugins/ClassFieldSetterPlugin.h"
#include "generator/translator_plugins/ContainerFieldPlugin.h"

namespace holgen {

  Translator::Translator(const ProjectDefinition &project) : mProject(project) {
    Validator(project).Validate();
    AddPlugin<ClassPlugin>();
    AddPlugin<ClassFieldPlugin>();
    AddPlugin<ClassFieldGetterPlugin>();
    AddPlugin<ClassFieldSetterPlugin>();
    AddPlugin<ContainerFieldPlugin>();
  }

  TranslatedProject Translator::Translate() {


    for (const auto &plugin: mPlugins) {
      plugin->EnrichClasses();
    }

    for (auto& cls: mProject.mClasses) {
      if (cls.mStruct)
        GenerateClass(cls, *cls.mStruct);
      else if (cls.mEnum)
        GenerateEnum(cls, *cls.mEnum);
    }

    // Enrichment should not create new classes. It can add new fields though.
    // Enrichers should check StructDefinition for user fields, the ones in Class may be created by enrichment
    GeneratorJson generatorJson(mProject);
    generatorJson.EnrichClasses();
    GeneratorLua generatorLua(mProject);
    generatorLua.EnrichClasses();
    GeneratorGlobalPointer generatorGlobalPointer(mProject);
    generatorGlobalPointer.EnrichClasses();
    GeneratorFilesystemHelper generatorFilesystemHelper(mProject);
    generatorFilesystemHelper.EnrichClasses();

    for (const auto &plugin: mPlugins) {
      plugin->GenerateHelpers();
    }


    // After all integrations processed all real structs, create helpers
    generatorJson.GenerateHelpers();
    generatorLua.GenerateHelpers();
    generatorGlobalPointer.GenerateHelpers();
    generatorFilesystemHelper.GenerateHelpers();

    return mProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition __attribute__((unused))) const {
    ProcessManagedClass(generatedClass);
  }

  void Translator::ProcessManagedClass(Class &generatedClass) const {
    if (generatedClass.mStruct == nullptr)
      return;
    auto managedAnnotation = generatedClass.mStruct->GetAnnotation(Annotations::Managed);
    if (managedAnnotation == nullptr)
      return;
    auto managedByAttribute = managedAnnotation->GetAttribute(Annotations::Managed_By);
    auto managedFieldAttribute = managedAnnotation->GetAttribute(Annotations::Managed_Field);
    auto manager = mProject.mProject.GetStruct(managedByAttribute->mValue.mName);
    auto managerField = manager->GetField(managedFieldAttribute->mValue.mName);
    auto managerFieldContainerAnnotation = managerField->GetAnnotation(Annotations::Container);
    auto managerFieldContainerElemNameAttribute = managerFieldContainerAnnotation->GetAttribute(
        Annotations::Container_ElemName);
    // auto managerFieldContainerConstAttribute = managerFieldContainerAnnotation->GetAttribute( Annotations::Container_Const);
    auto idField = generatedClass.mStruct->GetIdField();

    auto &getter = generatedClass.mMethods.emplace_back();
    getter.mName = St::ManagedObject_Getter;
    getter.mStaticness = Staticness::Static;
    getter.mConstness = Constness::NotConst;
    getter.mReturnType.mName = generatedClass.mStruct->mName;
    getter.mReturnType.mType = PassByType::Pointer;
    getter.mReturnType.mConstness = Constness::NotConst;
    auto &idArg = getter.mArguments.emplace_back();
    TypeInfo::Get().ConvertToType(idArg.mType, idField->mType);
    idArg.mName = "id";
    getter.mBody.Add("return {}<{}>::GetInstance()->{}(id);",
                     St::GlobalPointer, manager->mName,
                     St::GetGetterMethodName(managerFieldContainerElemNameAttribute->mValue.mName));
    generatedClass.mSourceIncludes.AddLocalHeader(St::GlobalPointer + ".h");
    generatedClass.mSourceIncludes.AddLocalHeader(manager->mName + ".h");

    for (auto &annotation: managerField->mAnnotations) {
      if (annotation.mName != Annotations::Index)
        continue;

      auto &getter = generatedClass.mMethods.emplace_back();
      auto indexOn = annotation.GetAttribute(Annotations::Index_On);
      getter.mName = St::GetIndexGetterName("", indexOn->mValue.mName);
      getter.mStaticness = Staticness::Static;
      getter.mConstness = Constness::NotConst;
      getter.mReturnType.mName = generatedClass.mStruct->mName;
      getter.mReturnType.mType = PassByType::Pointer;
      getter.mReturnType.mConstness = Constness::NotConst;
      ClassMethodArgument &valArg = getter.mArguments.emplace_back();
      auto indexedOnField = generatedClass.mStruct->GetField(indexOn->mValue.mName);
      TypeInfo::Get().ConvertToType(valArg.mType, indexedOnField->mType);
      if (!TypeInfo::Get().CppPrimitives.contains(valArg.mType.mName)) {
        valArg.mType.mType = PassByType::Reference;
        valArg.mType.mConstness = Constness::Const;
      }
      valArg.mName = "val";
      getter.mBody.Add(
          "return {}<{}>::GetInstance()->{}(val);",
          St::GlobalPointer, manager->mName,
          St::GetIndexGetterName(managerFieldContainerElemNameAttribute->mValue.mName, indexOn->mValue.mName));

    }
  }

  void Translator::GenerateEnum(Class &generatedClass, const EnumDefinition &enumDefinition) const {
    {
      auto &typdef = generatedClass.mTypedefs.emplace_back();
      typdef.mTargetType = "UnderlyingType";
      typdef.mSourceType.mName = "int64_t";
    }
    {
      ClassField &valueField = generatedClass.mFields.emplace_back();
      valueField.mName = "mValue";
      valueField.mType.mName = "UnderlyingType";
    }
    {
      ClassConstructor &ctor = generatedClass.mConstructors.emplace_back();
      ctor.mExplicitness = Explicitness::Explicit;
      ClassMethodArgument &value = ctor.mArguments.emplace_back();
      value.mName = "value";
      value.mType.mName = "UnderlyingType";
      value.mDefaultValue = "Invalid";
      ClassConstructorInitializer &initializer = ctor.mInitializerList.emplace_back();
      initializer.mDestination = "mValue";
      initializer.mValue = "value";
    }

    for (auto &entry: enumDefinition.mEntries) {
      ClassField &valueField = generatedClass.mFields.emplace_back();
      valueField.mType.mConstness = Constness::Const;
      valueField.mType.mConstexprness = Constexprness::Constexpr;
      valueField.mStaticness = Staticness::Static;
      valueField.mType.mName = "UnderlyingType";
      valueField.mName = entry.mName + "Value";
      valueField.mDefaultValue = entry.mValue;
      valueField.mVisibility = Visibility::Public;

      ClassField &entryField = generatedClass.mFields.emplace_back();
      entryField.mType.mConstness = Constness::Const;
      entryField.mStaticness = Staticness::Static;
      entryField.mType.mName = generatedClass.mName;
      entryField.mName = entry.mName;
      entryField.mDefaultConstructorArguments.push_back(entry.mValue);
      entryField.mVisibility = Visibility::Public;
    }

    {
      ClassField &invalidEntry = generatedClass.mFields.emplace_back();
      invalidEntry.mType.mConstness = Constness::Const;
      invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
      invalidEntry.mStaticness = Staticness::Static;
      invalidEntry.mType.mName = "UnderlyingType";
      invalidEntry.mName = "Invalid";
      invalidEntry.mDefaultValue = enumDefinition.mInvalidValue;
      invalidEntry.mVisibility = Visibility::Public;
    }

    {
      ClassMethod &fromString = generatedClass.mMethods.emplace_back();
      fromString.mName = "FromString";
      fromString.mStaticness = Staticness::Static;
      fromString.mConstness = Constness::NotConst;
      fromString.mReturnType.mName = generatedClass.mName;
      ClassMethodArgument &strArg = fromString.mArguments.emplace_back();
      strArg.mType.mName = "std::string_view";
      strArg.mName = "str";
      bool isFirst = true;
      for (auto &entry: enumDefinition.mEntries) {
        if (isFirst) {
          isFirst = false;
          fromString.mBody.Add("if (str == \"{}\") {{", entry.mName);
        } else {
          fromString.mBody.Add("}} else if (str == \"{}\") {{", entry.mName);
        }
        fromString.mBody.Indent(1);
        fromString.mBody.Add("return {}({});", generatedClass.mName, entry.mValue);
        fromString.mBody.Indent(-1);
      }
      if (!enumDefinition.mEntries.empty()) {
        fromString.mBody.Add("}} else {{");
        fromString.mBody.Indent(1);
      }
      fromString.mBody.Add("return {}({}::Invalid);", generatedClass.mName, generatedClass.mName);
      if (!enumDefinition.mEntries.empty()) {
        fromString.mBody.Indent(-1);
        fromString.mBody.Add("}}");
      }
    }

    {
      ClassMethod &toString = generatedClass.mMethods.emplace_back();
      toString.mName = "ToString";
      toString.mConstness = Constness::Const;
      toString.mReturnType.mName = "std::string";
      toString.mBody.Add("switch (mValue) {{");
      toString.mBody.Indent(1);
      for (auto &entry: enumDefinition.mEntries) {
        toString.mBody.Add("case {}: return \"{}\";", entry.mValue, entry.mName);
      }
      toString.mBody.Add("default: return \"INVALID\";");
      toString.mBody.Indent(-1);
      toString.mBody.Add("}}"); // switch
    }

    std::vector<std::pair<std::string, std::pair<bool, bool>>> operations = {
        {"=",  {false, false}},
        {"==", {true,  true}},
        {"!=", {true,  true}}
    };
    for (int i = 0; i < 2; ++i) {
      bool isForIntegral = i == 0;
      for (auto&[op, settings]: operations) {
        auto&[isConst, returnResult] = settings;
        ClassMethod &opMethod = generatedClass.mMethods.emplace_back();
        opMethod.mName = "operator " + op;
        if (returnResult)
          opMethod.mReturnType.mName = "bool";
        else {
          opMethod.mReturnType.mName = generatedClass.mName;
          opMethod.mReturnType.mType = PassByType::Reference;
        }
        auto &rhs = opMethod.mArguments.emplace_back();
        rhs.mName = "rhs";
        if (isForIntegral) {
          rhs.mType.mName = "UnderlyingType";
        } else {
          rhs.mType.mConstness = Constness::Const;
          rhs.mType.mName = generatedClass.mName;
          rhs.mType.mType = PassByType::Reference;
        }
        opMethod.mConstness = isConst ? Constness::Const : Constness::NotConst;
        {
          auto line = opMethod.mBody.Line();
          if (returnResult)
            line << "return ";
          line << "mValue";
          line << " " << op << " rhs";
          if (!isForIntegral)
            line << ".mValue";
          line << ";";
        }
        if (!returnResult)
          opMethod.mBody.Add("return *this;");
      }
    }

  }
}
