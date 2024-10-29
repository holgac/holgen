#include "DeferredDeleterPlugin.h"

namespace holgen {

void DeferredDeleterPlugin::Run() {
  auto &cls = GenerateClass();
  GenerateField(cls);
  GenerateConstructor(cls);
  GeneratePerformMethod(cls);
}

Class &DeferredDeleterPlugin::GenerateClass() {
  auto cls = Class{"DeferredDeleter", mSettings.mNamespace};
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
  return mProject.mClasses.back();
}

void DeferredDeleterPlugin::GenerateField(Class &cls) {
  auto field = ClassField{Naming().FieldNameInCpp("func"), Type{"std::function"},
                          Visibility::Private, Staticness::NotStatic, "nullptr"};
  field.mType.mFunctionalTemplateParameters.emplace_back("void");
  field.mType.mFunctionalTemplateParameters.emplace_back("void", PassByType::Pointer);
  Validate().NewField(cls, field);
  cls.mFields.push_back(std::move(field));
}

void DeferredDeleterPlugin::GeneratePerformMethod(Class &cls) {
  auto method = ClassMethod{"Perform", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mArguments.emplace_back("ptr", Type{cls.mName, PassByType::Pointer});
  method.mBody.Add(
      "constexpr size_t ObjectOffset = sizeof(DeferredDeleter) + (sizeof(DeferredDeleter)%8);");
  method.mBody.Add("ptr->{}(reinterpret_cast<char*>(ptr) + ObjectOffset);",
                   Naming().FieldNameInCpp("func"));
  method.mBody.Add("delete ptr;");
  method.mExposeToScript = true;
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void DeferredDeleterPlugin::GenerateConstructor(Class &cls) {
  auto ctor = ClassConstructor{};
  auto &arg = ctor.mArguments.emplace_back("func", cls.mFields.front().mType);
  arg.mType.mType = PassByType::Reference;
  arg.mType.mConstness = Constness::Const;
  ctor.mExplicitness = Explicitness::Explicit;
  ctor.mInitializerList.emplace_back(Naming().FieldNameInCpp("func"), "func");

  cls.mConstructors.push_back(std::move(ctor));
}
} // namespace holgen
