#include "ClassEqualsOperatorPlugin.h"

namespace holgen {
  void ClassEqualsOperatorPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      ProcessClass(cls);
    }
  }

  void ClassEqualsOperatorPlugin::ProcessClass(Class &cls) {
    auto method = ClassMethod{"operator==", Type{"bool"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});

    if (cls.mFields.empty()) {
      method.mBody.Add("return true;");
    } else {
      method.mBody.Add("return");
      method.mBody.Indent(2);
      ssize_t fieldsLeft = cls.mFields.size() - 1;
      for (auto &field: cls.mFields) {
        if (TypeInfo::Get().FloatingPointTypes.contains(field.mType.mName)) {
          cls.mSourceIncludes.AddStandardHeader("cmath");
          method.mBody.Add("std::fabs({0} - rhs.{0}) < {1}{2}", field.mName, GetEpsilon(cls, field), fieldsLeft == 0 ? ";" : " &&");
        } else {
          method.mBody.Add("{0} == rhs.{0}{1}", field.mName, fieldsLeft == 0 ? ";" : " &&");
        }
        fieldsLeft -= 1;
      }
      method.mBody.Indent(-2);
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  std::string ClassEqualsOperatorPlugin::GetEpsilon(const Class &cls, const ClassField &field) {
    if (field.mField) {
      auto annotation = field.mField->GetAnnotation(Annotations::Epsilon);
      if (annotation) {
        return annotation->mAttributes.front().mName;
      }
    }
    if (cls.mStruct) {
      auto annotation = cls.mStruct->GetAnnotation(Annotations::Epsilon);
      if (annotation) {
        return annotation->mAttributes.front().mName;
      }
    }
    return mSettings.mEpsilon;
  }
}
