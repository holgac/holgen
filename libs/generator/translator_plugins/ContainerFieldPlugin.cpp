#include "ContainerFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void ContainerFieldPlugin::Run() {
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
          auto &indexField = generatedClass.mFields.emplace_back(
              St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName), Type{"std::map"});
          auto indexType = dec.GetAttribute(Annotations::Index_Using);
          if (indexType != nullptr) {
            indexField.mType = Type{indexType->mValue};
          }

          indexField.mType.mTemplateParameters.emplace_back(fieldIndexedOn.mType);
          indexField.mType.mTemplateParameters.emplace_back(underlyingIdField->mType);

          for (int i = 0; i < 2; ++i) {
            Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
            auto &func = generatedClass.mMethods.emplace_back(
                St::GetIndexGetterName(elemName->mValue.mName, indexOn->mValue.mName),
                Type{underlyingType, PassByType::Pointer, constness},
                Visibility::Public,
                constness);
            auto &arg = func.mArguments.emplace_back(
                "key", Type{fieldIndexedOn.mType});
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
          auto &func = generatedClass.mMethods.emplace_back(
              St::GetAdderMethodName(elemName->mValue.mName),
              Type{"bool"},
              isConstContainer ? Visibility::Private : Visibility::Public,
              Constness::NotConst
          );
          auto &arg = func.mArguments.emplace_back("elem", Type{underlyingType, PassByType::MoveReference});

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
          auto constness = i == 0 ? Constness::Const : Constness::NotConst;
          auto &func = generatedClass.mMethods.emplace_back(
              St::GetGetterMethodName(elemName->mValue.mName),
              Type{underlyingType, PassByType::Pointer, constness},
              Visibility::Public,
              constness
          );
          auto &arg = func.mArguments.emplace_back("idx", Type{underlyingIdField->mType});
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
