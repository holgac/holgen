#include "GlobalPointerPlugin.h"
#include "core/St.h"

namespace holgen {
void GlobalPointerPlugin::Run() {
  auto cls = Class{St::GlobalPointer, mSettings.mNamespace};
  cls.mTemplateParameters.emplace_back("typename", "T");

  GenerateInstanceField(cls);
  GenerateGetInstance(cls);
  GenerateSetInstance(cls);

  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void GlobalPointerPlugin::GenerateInstanceField(Class &cls) const {
  auto field = ClassField{
      "mInstance", Type{"T", PassByType::Pointer}, Visibility::Private,
      Staticness::Static, "nullptr"};
  Validate().NewField(cls, field);
  cls.mFields.push_back(std::move(field));
}

void GlobalPointerPlugin::GenerateGetInstance(Class &cls) const {
  auto method = ClassMethod{
      St::GlobalPointer_GetInstance, Type{"T", PassByType::Pointer},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mBody.Add("return mInstance;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void GlobalPointerPlugin::GenerateSetInstance(Class &cls) const {
  auto method = ClassMethod{
      St::GlobalPointer_SetInstance, Type{"void"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("ptr", Type{"T", PassByType::Pointer});
  // the users handle storage and deletion?
  method.mBody.Add("mInstance = ptr;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
}
