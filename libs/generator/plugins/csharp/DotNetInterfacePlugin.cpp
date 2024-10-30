#include "DotNetInterfacePlugin.h"

#include "core/St.h"

namespace holgen {

void DotNetInterfacePlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::CSharp))
    return;
  GenerateForCpp();
  GenerateForCSharp();
}

void DotNetInterfacePlugin::GenerateForCpp() {
  auto &cls = GenerateClass();
  GenerateConstructor(cls);
  GenerateDestructor(cls);
  GenerateField(cls);
  GenerateFieldGetter(cls);
  GenerateFreeMethod(cls);
  GenerateFreeMethodPtr(cls);
}

Class &DotNetInterfacePlugin::GenerateClass() {
  auto &cls = mProject.mClasses.emplace_back(St::CSharpInterfaceName, mSettings.mNamespace);
  cls.mFriendClasses.emplace_back(St::DotNetHostName);
  cls.mHeaderIncludes.AddForwardDeclaration({mSettings.mNamespace, "class", St::DotNetHostName});
  return cls;
}

void DotNetInterfacePlugin::GenerateConstructor(Class &cls) {
  auto ptrFieldName = Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName);
  {
    auto &ctor = cls.mConstructors.emplace_back();
    ctor.mArguments.emplace_back("ptr", Type{"void", PassByType::Pointer});
    ctor.mInitializerList.emplace_back(ptrFieldName, "ptr");
    ctor.mExplicitness = Explicitness::Explicit;
  }

  {
    auto &copyCtor = cls.mConstructors.emplace_back();
    copyCtor.mArguments.emplace_back("rhs",
                                     Type{cls.mName, PassByType::Reference, Constness::Const});
    copyCtor.mDefaultDelete = DefaultDelete::Delete;
  }
  {
    auto &copyOp = cls.mMethods.emplace_back(
        "operator=", Type{cls.mName, PassByType::Reference, Constness::Const});
    copyOp.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    copyOp.mDefaultDelete = DefaultDelete::Delete;
  }
  {
    auto &moveCtor = cls.mConstructors.emplace_back();
    moveCtor.mNoexceptness = Noexceptness::Noexcept;
    moveCtor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    moveCtor.mBody.Add("std::swap({0}, rhs.{0});", ptrFieldName);
  }
  {
    auto &moveOp = cls.mMethods.emplace_back("operator=", Type{cls.mName, PassByType::Reference},
                                             Visibility::Public, Constness::NotConst);
    moveOp.mNoexceptness = Noexceptness::Noexcept;
    moveOp.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    moveOp.mBody.Add("std::swap({0}, rhs.{0});", ptrFieldName);
    moveOp.mBody.Add("return *this;");
  }
}

void DotNetInterfacePlugin::GenerateDestructor(Class &cls) {
  cls.mDestructor.mBody.Add("{}();", St::CSharpInterfaceFree);
}

void DotNetInterfacePlugin::GenerateField(Class &cls) {
  cls.mFields.emplace_back(Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName),
                           Type{"void", PassByType::Pointer}, Visibility::Protected,
                           Staticness::NotStatic, "nullptr");
}

void DotNetInterfacePlugin::GenerateFieldGetter(Class &cls) {
  auto &method = cls.mMethods.emplace_back(
      Naming().FieldGetterNameInCpp(St::CSharpInterfaceInstanceName),
      Type{"void", PassByType::Pointer, Constness::NotConst}, Visibility::Public, Constness::Const);
  method.mBody.Add("return {};", Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName));
}

void DotNetInterfacePlugin::GenerateFreeMethod(Class &cls) {
  auto &method = cls.mMethods.emplace_back(St::CSharpInterfaceFree, Type{"void"},
                                           Visibility::Protected, Constness::NotConst);
  method.mVisibility = Visibility::Protected;
  method.mBody.Add("if ({}) {{", Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName));
  method.mBody.Indent(1);
  method.mBody.Add("{}({});", St::CSharpInterfaceFreePtr,
                   Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName));
  method.mBody.Add("{} = nullptr;", Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName));
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void DotNetInterfacePlugin::GenerateFreeMethodPtr(Class &cls) {
  auto &method =
      cls.mMethods.emplace_back(St::CSharpInterfaceFreePtr, Type{"void"}, Visibility::Private,
                                Constness::NotConst, Staticness::Static);
  method.mArguments.emplace_back("ptr", Type{"void", PassByType::Pointer});
  method.mStaticness = Staticness::Static;
  method.mFunctionPointer = true;
}

void DotNetInterfacePlugin::GenerateForCSharp() {
  auto &csCls = GenerateCSharpClass();
  GenerateCSharpFreeMethod(csCls);
}

CSharpClass &DotNetInterfacePlugin::GenerateCSharpClass() {
  auto &cls = mProject.mCSharpClasses.emplace_back(St::CSharpInterfaceName);
  cls.mUsingDirectives.emplace("System.Runtime.InteropServices");
  return cls;
}

void DotNetInterfacePlugin::GenerateCSharpFreeMethod(CSharpClass &cls) {
  auto method = CSharpMethod{Naming().CSharpMethodDelegateName(cls.mName, St::CSharpInterfaceFree),
                             CSharpType{"void"}, CSharpVisibility::Public, Staticness::Static};
  method.mArguments.emplace_back("ptr", CSharpType{"IntPtr"});
  cls.mDelegates.push_back(method);
  method.mName = St::CSharpInterfaceFree;
  method.mBody.Add("var handle = GCHandle.FromIntPtr(ptr);");
  method.mBody.Add("handle.Free();");
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
