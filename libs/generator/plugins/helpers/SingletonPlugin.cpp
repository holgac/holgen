#include "SingletonPlugin.h"

namespace holgen {

void SingletonPlugin::Run() {
  auto &cls = GenerateClass();
  GenerateField(cls);
  GenerateGetter(cls, true);
  GenerateGetter(cls, false);
  GenerateSetter(cls);
  GenerateDeleter(cls);
}

Class &SingletonPlugin::GenerateClass() {
  auto cls = Class{"Singleton", mSettings.mNamespace};
  cls.mTemplateParameters.emplace_back("typename", "T");
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
  return mProject.mClasses.back();
}

void SingletonPlugin::GenerateField(Class &cls) {
  auto field = ClassField{Naming().FieldNameInCpp("instance"), Type{"T", PassByType::Pointer},
                          Visibility::Private, Staticness::Static, "nullptr"};
  Validate().NewField(cls, field);
  cls.mFields.push_back(std::move(field));
}

void SingletonPlugin::GenerateGetter(Class &cls, bool pointer) {
  auto method = ClassMethod{pointer ? "GetSingleton" : "GetInstance",
                            Type{"T", pointer ? PassByType::Pointer : PassByType::Reference},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mBody.Add("return {}{};", pointer ? "" : "*", Naming().FieldNameInCpp("instance"));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void SingletonPlugin::GenerateSetter(Class &cls) {
  auto method = ClassMethod{"SetSingleton", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mArguments.emplace_back("val", Type{"T", PassByType::Pointer});
  method.mBody.Add(
      "HOLGEN_FAIL_IF({}, \"SetSingleton called on a class that already has an instance!\");",
      Naming().FieldNameInCpp("instance"));
  method.mBody.Add("{} = val;", Naming().FieldNameInCpp("instance"));

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void SingletonPlugin::GenerateDeleter(Class &cls) {
  auto method = ClassMethod{"DeleteSingleton", Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mBody.Add(
      "HOLGEN_FAIL_IF(!{}, \"DeleteSingleton called on a class that does not have an instance!\");",
      Naming().FieldNameInCpp("instance"));
  method.mBody.Add("delete {};", Naming().FieldNameInCpp("instance"));
  method.mBody.Add("{} = nullptr;", Naming().FieldNameInCpp("instance"));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
