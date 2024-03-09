#include "ContainerFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void ContainerFieldPlugin::EnrichClasses() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto container = fieldDefinition.GetAnnotation(Annotations::Container);
        if (!container)
          continue;
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
          func.mBody.Add("{}.back().{}(newId);", generatedField.mName,
                         St::GetSetterMethodName(underlyingIdField->mName));
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
    }
  }
}
