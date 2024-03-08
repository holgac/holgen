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

namespace holgen {

  Translator::Translator(const ProjectDefinition &project) : mProject(project) {
    Validator(project).Validate();
  }

  TranslatedProject Translator::Translate() {
    for (auto &structDefinition: mProject.mProject.mStructs) {
      GenerateClass(mProject.mClasses.emplace_back(), structDefinition);
    }
    for (auto &enumDefinition: mProject.mProject.mEnums) {
      GenerateEnum(mProject.mClasses.emplace_back(), enumDefinition);
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


    // After all integrations processed all real structs, create helpers
    generatorJson.GenerateHelpers();
    generatorLua.GenerateHelpers();
    generatorGlobalPointer.GenerateHelpers();
    generatorFilesystemHelper.GenerateHelpers();

    return mProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    generatedClass.mStruct = &structDefinition;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }

    ProcessManagedClass(generatedClass);
  }

  void Translator::ProcessRefField(Class &generatedClass, ClassField &generatedField,
                                   const FieldDefinition &fieldDefinition) const {
    if (!fieldDefinition.mDefaultValue.empty())
      generatedField.mDefaultValue = fieldDefinition.mDefaultValue;
    else
      generatedField.mDefaultValue = "-1";
    auto refStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
    auto refStructId = refStruct->GetIdField();
    TypeInfo::Get().ConvertToType(generatedField.mType, refStructId->mType);
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Add("return {}::{}({});", refStruct->mName, St::ManagedObject_Getter,
                       generatedField.mName);

      TypeInfo::Get().ConvertToType(getter.mReturnType, fieldDefinition.mType.mTemplateParameters[0]);
      getter.mConstness = constness;
      getter.mReturnType.mConstness = constness;
      getter.mReturnType.mType = PassByType::Pointer;
    }
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();
    generatedField.mField = &fieldDefinition;

    bool isRef = fieldDefinition.mType.mName == "Ref";
    generatedField.mName = St::GetFieldNameInCpp(fieldDefinition.mName, isRef);
    if (isRef) {
      ProcessRefField(generatedClass, generatedField, fieldDefinition);
    } else {
      generatedField.mDefaultValue = fieldDefinition.mDefaultValue;
      TypeInfo::Get().ConvertToType(generatedField.mType, fieldDefinition.mType);
    }

    bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);
    {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName, isRef);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mReturnType = generatedField.mType;
      getter.mConstness = Constness::Const;
      if (!isPrimitive) {
        getter.mReturnType.mConstness = Constness::Const;
        getter.mReturnType.mType = PassByType::Reference;
      }
    }

    // non-const getter for non-primitives only
    if (!isPrimitive) {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName, isRef);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mReturnType = generatedField.mType;
      getter.mConstness = Constness::NotConst;
      getter.mReturnType.mType = PassByType::Reference;
    }

    {
      auto &setter = generatedClass.mMethods.emplace_back();
      setter.mName = St::GetSetterMethodName(fieldDefinition.mName, isRef);
      setter.mConstness = Constness::NotConst;
      auto &arg = setter.mArguments.emplace_back();
      arg.mType = generatedField.mType;
      if (!isPrimitive) {
        arg.mType.mConstness = Constness::Const;
        arg.mType.mType = PassByType::Reference;
      }
      arg.mName = "val";
      setter.mBody.Line() << generatedField.mName << " = val;";
      setter.mReturnType.mName = "void";
    }

    ProcessContainerField(generatedClass, fieldDefinition);
  }

  void Translator::ProcessContainerField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto container = fieldDefinition.GetAnnotation(Annotations::Container);
    if (!container)
      return;
    bool isConstContainer = container->GetAttribute(Annotations::Container_Const) != nullptr;
    auto elemName = container->GetAttribute(Annotations::Container_ElemName);
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters[0];
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();

    for (auto &dec: fieldDefinition.mAnnotations) {
      if (dec.mName != Annotations::Index)
        continue;
      auto indexOn = dec.GetAttribute(Annotations::Index_On);
      auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
      auto &indexField = generatedClass.mFields.emplace_back();
      indexField.mName = St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName);
      auto indexType = dec.GetAttribute(Annotations::Index_Using);
      if (indexType != nullptr) {
        TypeInfo::Get().ConvertToType(indexField.mType, indexType->mValue);
      } else {
        indexField.mType.mName = "std::map";
      }

      auto &indexKey = indexField.mType.mTemplateParameters.emplace_back();
      TypeInfo::Get().ConvertToType(indexKey, fieldIndexedOn.mType);
      auto &indexValue = indexField.mType.mTemplateParameters.emplace_back();
      TypeInfo::Get().ConvertToType(indexValue, underlyingIdField->mType);

      for (int i = 0; i < 2; ++i) {
        Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
        auto &func = generatedClass.mMethods.emplace_back();
        func.mName = St::GetIndexGetterName(elemName->mValue.mName, indexOn->mValue.mName);
        func.mConstness = constness;
        TypeInfo::Get().ConvertToType(func.mReturnType, underlyingType);
        func.mReturnType.mConstness = constness;
        func.mReturnType.mType = PassByType::Pointer;
        auto &arg = func.mArguments.emplace_back();
        arg.mName = "key";
        TypeInfo::Get().ConvertToType(arg.mType, fieldIndexedOn.mType);
        if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
          arg.mType.mConstness = Constness::Const;
          arg.mType.mType = PassByType::Reference;
        }

        func.mBody.Add("auto it = {}.find(key);", indexField.mName);
        func.mBody.Add("if (it == {}.end())", indexField.mName);
        func.mBody.Indent(1);
        func.mBody.Add("return nullptr;");
        func.mBody.Indent(-1);
        func.mBody.Add("return &{}[it->second];", St::GetFieldNameInCpp(fieldDefinition.mName));

      }
    }

    // Getting generatedField here because we added a new field above which would've invalidated it
    auto &generatedField = *generatedClass.GetField(St::GetFieldNameInCpp(fieldDefinition.mName));

    {
      // Generate AddElem
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetAdderMethodName(elemName->mValue.mName);
      func.mConstness = Constness::NotConst;
      func.mReturnType.mName = "bool";
      if (isConstContainer)
        func.mVisibility = Visibility::Private;
      else
        func.mVisibility = Visibility::Public;
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingType);
      arg.mType.mConstness = Constness::NotConst;
      arg.mType.mType = PassByType::MoveReference;
      arg.mName = "elem";

      func.mBody.Line() << "auto newId = " << generatedField.mName << ".size();";
      CodeBlock validators;
      CodeBlock inserters;
      for (auto &dec: fieldDefinition.mAnnotations) {
        if (dec.mName != Annotations::Index)
          continue;
        auto indexOn = dec.GetAttribute(Annotations::Index_On);
        auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
        auto indexFieldName = St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName);
        auto getterMethodName = St::GetGetterMethodName(fieldIndexedOn.mName);
        validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
        validators.Indent(1);
        validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists!", elem.{}());)",
                       underlyingStructDefinition->mName, indexOn->mValue.mName, getterMethodName);
        validators.Add("return false;");
        validators.Indent(-1);
        validators.Add("}}");
        inserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
      }
      func.mBody.Add(validators);
      func.mBody.Add(inserters);
      func.mBody.Add("{}.emplace_back(std::forward<{}>(elem));", generatedField.mName, arg.mType.mName);
      func.mBody.Add("{}.back().{}(newId);", generatedField.mName, St::GetSetterMethodName(underlyingIdField->mName));
      func.mBody.Line() << "return true;";
    }

    for (int i = 0; i < 2; ++i) {
      bool isConst = i == 0;
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetGetterMethodName(elemName->mValue.mName);
      func.mConstness = isConst ? Constness::Const : Constness::NotConst;
      TypeInfo::Get().ConvertToType(func.mReturnType, underlyingType);
      func.mReturnType.mType = PassByType::Pointer;
      func.mReturnType.mConstness = isConst ? Constness::Const : Constness::NotConst;
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingIdField->mType);
      arg.mType.mConstness = Constness::NotConst;
      arg.mName = "idx";
      {
        auto line = func.mBody.Line();
        line << "if (idx >= " << generatedField.mName << ".size()";
        if (TypeInfo::Get().SignedIntegralTypes.contains(arg.mType.mName)) {
          line << "|| idx < 0";
        }
        line << ")";
      }
      func.mBody.Indent(1);
      func.mBody.Line() << "return nullptr;";
      func.mBody.Indent(-1);
      func.mBody.Line() << "return &" << generatedField.mName << "[idx];";
    }

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
    generatedClass.mEnum = &enumDefinition;
    generatedClass.mName = enumDefinition.mName;
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
