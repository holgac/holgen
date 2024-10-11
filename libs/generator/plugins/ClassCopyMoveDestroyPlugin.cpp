#include "ClassCopyMoveDestroyPlugin.h"

#include "core/St.h"

#include "core/Exception.h"

namespace holgen {
void ClassCopyMoveDestroyPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct) {
      continue;
    }
    ProcessClass(cls);
  }
}

void ClassCopyMoveDestroyPlugin::ProcessClass(Class &cls) {
  auto variantData = cls.GetVariantData();
  bool isCopyable = Type{cls.mName}.IsCopyable(mProject);
  bool needsCustomCopy = NeedsCustomCopyOperator(cls);

  auto defaultConstructor = ClassConstructor{};
  defaultConstructor.mDefaultDelete = DefaultDelete::Default;
  cls.mConstructors.push_back(std::move(defaultConstructor));

  auto copyAssignment = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference},
                                    Visibility::Public, Constness::NotConst};
  copyAssignment.mArguments.emplace_back("rhs",
                                         Type{cls.mName, PassByType::Reference, Constness::Const});

  if (!isCopyable) {
    copyAssignment.mDefaultDelete = DefaultDelete::Delete;
  } else if (needsCustomCopy) {
    PopulateMethod(cls, copyAssignment, variantData, false, true);
  } else {
    copyAssignment.mDefaultDelete = DefaultDelete::Default;
  }

  Validate().NewMethod(cls, copyAssignment);
  cls.mMethods.push_back(std::move(copyAssignment));

  auto copyConstructor = ClassConstructor{};
  copyConstructor.mArguments.emplace_back("rhs",
                                          Type{cls.mName, PassByType::Reference, Constness::Const});
  if (!isCopyable) {
    copyConstructor.mDefaultDelete = DefaultDelete::Delete;
  } else if (needsCustomCopy) {
    PopulateMethod(cls, copyConstructor, variantData, false, false);
  } else {
    copyConstructor.mDefaultDelete = DefaultDelete::Default;
  }

  cls.mConstructors.push_back(std::move(copyConstructor));

  auto moveAssignment = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference},
                                    Visibility::Public, Constness::NotConst};
  moveAssignment.mNoexceptness = Noexceptness::Noexcept;
  moveAssignment.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
  if (!isCopyable || needsCustomCopy) {
    PopulateMethod(cls, moveAssignment, variantData, true, true);
  } else {
    moveAssignment.mDefaultDelete = DefaultDelete::Default;
  }
  Validate().NewMethod(cls, moveAssignment);
  cls.mMethods.push_back(std::move(moveAssignment));

  auto moveConstructor = ClassConstructor{};
  moveConstructor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
  if (!isCopyable || needsCustomCopy) {
    PopulateMethod(cls, moveConstructor, variantData, true, false);
  } else {
    moveConstructor.mDefaultDelete = DefaultDelete::Default;
  }
  moveConstructor.mNoexceptness = Noexceptness::Noexcept;
  cls.mConstructors.push_back(std::move(moveConstructor));
  if (cls.mDestructor.IsEmpty()) {
    cls.mDestructor.mDefaultDelete = DefaultDelete::Default;
  }
}

void ClassCopyMoveDestroyPlugin::PopulateMethod(Class &cls, ClassMethodBase &method,
                                                VariantData &variantData, bool isMove,
                                                bool returnThis) {
  // First handle variants, if any
  PopulateMethodVariantFields(cls, method, variantData, isMove);
  for (auto &field: cls.mFields) {
    if (field.mField &&
        (variantData.contains(field.mField->mName) ||
         field.mField->GetAnnotation(Annotations::Variant))) {
      continue;
    }

    if (field.mField && field.mField->mType.mName == St::Lua_CustomData) {
      THROW_IF(!isMove, "Lua custom data cannot be copied");
      method.mBody.Add("std::swap({0}, rhs.{0});", field.mName);
    } else if (field.mField && field.mField->mType.mName == St::UserData) {
      auto onCopy =
          field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_OnCopy);
      if (isMove || onCopy->mValue.mName == Annotations::Field_OnCopy_Swap) {
        method.mBody.Add("std::swap({0}, rhs.{0});", field.mName);
      } else if (onCopy && onCopy->mValue.mName == Annotations::Field_OnCopy_Copy) {
        method.mBody.Add("{0} = rhs.{0};", field.mName);
      } else {
        THROW_IF(!isMove, "Userdata {} cannot be copied unless copying is explicitly allowed",
                 field.mField->mDefinitionSource);
        method.mBody.Add("std::swap({0}, rhs.{0});", field.mName);
      }
    } else if (isMove && !TypeInfo::Get().CppPrimitives.contains(field.mType.mName)) {
      method.mBody.Add("{0} = std::move(rhs.{0});", field.mName);
    } else if (isMove && field.mType.mType == PassByType::Pointer) {
      method.mBody.Add("std::swap({0}, rhs.{0});", field.mName);
    } else {
      method.mBody.Add("{0} = rhs.{0};", field.mName);
    }
  }
  if (returnThis) {
    method.mBody.Add("return *this;");
  }
}

bool ClassCopyMoveDestroyPlugin::NeedsCustomCopyOperator(Class &cls) {
  for (auto &field: cls.mFields) {
    if (field.mField && field.mField->GetAnnotation(Annotations::Variant)) {
      return true;
    }
  }
  return false;
}

void ClassCopyMoveDestroyPlugin::PopulateMethodVariantFields(Class &cls, ClassMethodBase &method,
                                                             VariantData &variantData,
                                                             bool isMove) {
  for (auto &[variantTypeFieldName, variantFields]: variantData) {
    method.mBody.Add("{}();", Naming().VariantResetterNameInCpp(variantTypeFieldName));
    method.mBody.Add("{}(rhs.{});", Naming().FieldSetterNameInCpp(variantTypeFieldName),
                     Naming().FieldNameInCpp(variantTypeFieldName));
    auto variantEnumName = cls.GetField(Naming().FieldNameInCpp(variantTypeFieldName))->mType.mName;
    auto variantClasses = mProject.GetVariantClassesOfEnum(variantEnumName);

    bool isFirst = true;
    for (auto &[variantClass, enumEntry]: variantClasses) {
      if (isFirst) {
        method.mBody.Add("if ({} == {}::{}) {{", Naming().FieldNameInCpp(variantTypeFieldName),
                         variantEnumName, enumEntry->mName);
        isFirst = false;
      } else {
        method.mBody.Add("}} else if ({} == {}::{}) {{",
                         Naming().FieldNameInCpp(variantTypeFieldName), variantEnumName,
                         enumEntry->mName);
      }
      method.mBody.Indent(1);
      for (auto &variantField: variantFields) {
        if (isMove) {
          method.mBody.Add(
              "*{0}() = std::move(*rhs.{0}());",
              Naming().VariantGetterNameInCpp(*variantField->mField, *variantClass->mStruct));
        } else {
          method.mBody.Add(
              "*{0}() = *rhs.{0}();",
              Naming().VariantGetterNameInCpp(*variantField->mField, *variantClass->mStruct));
        }
      }
      method.mBody.Indent(-1);
    }
    method.mBody.Add("}}");

    if (isMove) {
      method.mBody.Add("rhs.{}();", Naming().VariantResetterNameInCpp(variantTypeFieldName));
    }
  }
}
} // namespace holgen
