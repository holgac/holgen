#include "ClassFieldSetterPlugin.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void ClassFieldSetterPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    for (auto &field: cls.mFields) {
      if (!field.mField)
        continue;
      if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Const))
        continue;
      if (field.mField->mType.mName == St::Variant)
        continue;
      if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Set,
                                             Annotations::MethodOption_None))
        continue;
      auto method = ClassMethod{
          Naming().FieldSetterNameInCpp(*field.mField), Type{"void"},
          Visibility::Public, Constness::NotConst};
      auto &arg = method.mArguments.emplace_back("val", field.mType);
      if (field.mType.mType == PassByType::Pointer)
        arg.mType.mType = PassByType::Pointer;
      else
        arg.mType.PreventCopying();
      if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Set,
                                             Annotations::MethodOption_Custom)) {
        method.mUserDefined = true;
        if (field.mField->mType.mName == St::UserData) {
          arg.mType.mType = PassByType::Pointer;
          arg.mType.mName = "void";
        }
      } else {
        if (field.mField->mType.mName == St::UserData) {
          method.mTemplateParameters.emplace_back("typename", "T");
          arg.mType.mName = "T";
          method.mBody.Add("{} = reinterpret_cast<void*>(val);", field.mName);
        } else {
          auto isNonCopyable = [&](const std::string& name) {
            auto underlyingClass = mProject.GetClass(name);
            if (underlyingClass && underlyingClass->mStruct && underlyingClass->mStruct->GetMatchingAttribute(Annotations::Struct, Annotations::Struct_NonCopyable)) {
              return true;
            }
            return false;
          };
          bool useMove = isNonCopyable(field.mType.mName);
          if (!useMove && TypeInfo::Get().CppContainers.contains(field.mType.mName)) {
            auto* underlyingType = &field.mType.mTemplateParameters.back().mName;
            if (field.mType.mName == "std::array")
              underlyingType = &field.mType.mTemplateParameters.front().mName;
            useMove = isNonCopyable(*underlyingType);
          }
          if (useMove) {
            arg.mType.mType = PassByType::MoveReference;
            arg.mType.mConstness = Constness::NotConst;
            method.mBody.Add("{} = std::move(val);", field.mName);
          } else {
            method.mBody.Add("{} = val;", field.mName);
          }
        }
      }
      if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Set,
                                             Annotations::MethodOption_Private)) {
        method.mVisibility = Visibility::Private;
      } else if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Set,
                                                    Annotations::MethodOption_Protected)) {
        method.mVisibility = Visibility::Protected;
      }
      Validate().NewMethod(cls, method);
      cls.mMethods.push_back(std::move(method));
    }
  }
}
}
