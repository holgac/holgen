#include "ClassEqualsOperatorPlugin.h"

namespace holgen {
void ClassEqualsOperatorPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr || cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    ProcessClass(cls);
  }
}

void ClassEqualsOperatorPlugin::ProcessClass(Class &cls) {
  auto method = ClassMethod{"operator==", Type{"bool"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
  auto variantData = cls.GetVariantData();
  std::set<std::string> variantFields;
  for (auto &[_, fields]: variantData) {
    for (auto &field: fields) {
      variantFields.insert(field->mName);
    }
  }

  if (cls.mFields.empty()) {
    method.mBody.Add("return true;");
  } else {
    if (variantData.empty()) {
      method.mBody.Add("return !(");
    } else {
      method.mBody.Add("if (");
    }
    method.mBody.Indent(2);
    std::vector<std::string> lines;
    for (auto &field: cls.mFields) {
      if (variantFields.contains(field.mName)) {
        continue;
      }

      if (TypeInfo::Get().FloatingPointTypes.contains(field.mType.mName)) {
        cls.mSourceIncludes.AddStandardHeader("cmath");
        lines.push_back(
            std::format("std::fabs({0} - rhs.{0}) >= {1}", field.mName, GetEpsilon(cls, field)));
      } else {
        lines.push_back(std::format("{0} != rhs.{0}", field.mName));
      }
    }
    for (auto &classMethod: cls.mMethods) {
      if (!classMethod.mFunctionPointer)
        continue;
      lines.push_back(std::format("{0} != rhs.{0}", classMethod.mName));
    }
    for (size_t i = 0; i < lines.size(); ++i) {
      method.mBody.Add("{}{}", lines[i], i == lines.size() - 1 ? "" : " ||");
    }
    if (variantData.empty()) {
      method.mBody.Indent(-2);
      method.mBody.Add(");");
    } else {
      method.mBody.Indent(-2);
      method.mBody.Add(") {{");
      method.mBody.Indent(1);
      method.mBody.Add("return false;");
      method.mBody.Indent(-1);
      method.mBody.Add("}}");

      for (auto &[typeFieldName, variantFields]: variantData) {
        bool isFirst = true;

        auto variantTypeField = cls.GetField(Naming().FieldNameInCpp(typeFieldName));
        auto variantClasses = mProject.GetVariantClassesOfEnum(variantTypeField->mType.mName);
        for (auto &[variantClass, enumEntry]: variantClasses) {
          if (isFirst) {
            method.mBody.Add("if ({} == {}::{}) {{", variantTypeField->mName,
                             variantTypeField->mType.mName, enumEntry->mName);
            isFirst = false;
          } else {
            method.mBody.Add("}} else if ({} == {}::{}) {{", variantTypeField->mName,
                             variantTypeField->mType.mName, enumEntry->mName);
          }
          method.mBody.Indent(1);
          for (auto &variantField: variantFields) {
            method.mBody.Add(
                "if (!(*{0}() == *rhs.{0}())) {{",
                Naming().VariantGetterNameInCpp(*variantField->mField, *variantClass->mStruct));
            method.mBody.Indent(1);
            method.mBody.Add("return false;");
            method.mBody.Indent(-1);
            method.mBody.Add("}}");
          }
          method.mBody.Indent(-1);
        }
        method.mBody.Add("}}");
        method.mBody.Add("return true;");
      }
    }
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
} // namespace holgen
