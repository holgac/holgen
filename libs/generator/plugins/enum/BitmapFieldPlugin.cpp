#include "BitmapFieldPlugin.h"

#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void BitmapFieldPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct) {
      ProcessClass(cls);
    }
  }
}

void BitmapFieldPlugin::ProcessClass(Class &cls) {
  for (auto &field: cls.mFields) {
    if (!field.mField) {
      continue;
    }
    auto fieldClass = mProject.GetClass(field.mType.mName);
    if (!fieldClass || !fieldClass->mEnum ||
        fieldClass->mEnum->mType != EnumDefinitionType::Bitmap) {
      continue;
    }

    for (auto &readMethod: {"Has"}) {
      AddMethod(cls, field, *fieldClass, readMethod, false);
    }
    for (auto &mutateMethod: {"Add", "Remove", "Toggle"}) {
      AddMethod(cls, field, *fieldClass, mutateMethod, true);
    }
  }
}

void BitmapFieldPlugin::AddMethod(Class &cls, const ClassField &field, const Class &fieldClass,
                                  const std::string &name, bool isMutator) {
  auto method = ClassMethod{std::format("{}{}", name, St::Capitalize(field.mField->mName)),
                            Type{isMutator ? "void" : "bool"}, Visibility::Public,
                            isMutator ? Constness::NotConst : Constness::Const};

  auto underlying = fieldClass.GetUsing(St::Enum_UnderlyingType);
  THROW_IF(!underlying, "Underlying type not defined")
  method.mArguments.emplace_back("val", underlying->mSourceType);
  method.mBody.Add("{}{}.{}({}(val));", isMutator ? "" : "return ", field.mName, name,
                   fieldClass.mName);

  method.mExposeToLua = true;
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
