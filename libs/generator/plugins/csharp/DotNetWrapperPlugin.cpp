#include "DotNetWrapperPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

#include "generator/utils/CSharpMethodHelper.h"

namespace holgen {
void DotNetWrapperPlugin::Run() {
  for (auto &csCls: mProject.mCSharpClasses) {
    if (!csCls.mClass || !ShouldProcess(*csCls.mClass))
      continue;
    Process(*csCls.mClass, csCls);
  }
}

void DotNetWrapperPlugin::Process(Class &cls, CSharpClass &csCls) const {
  csCls.mStaticness = IsStaticClass(cls) ? Staticness::Static : Staticness::NotStatic;
  csCls.mUsingDirectives.insert("System.Runtime.InteropServices");
  ProcessConstructors(cls, csCls);
  ProcessMethods(cls, csCls);
  ProcessMethodPointers(cls, csCls);
  if (cls.IsProxyable()) {
    if (csCls.mStaticness != Staticness::Static) {
      ProcessProxy(cls, csCls);
    }
  } else if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::Singleton)) {
    ProcessMirror(cls, csCls);
  }
  ProcessInitializer(cls, csCls);
}

void DotNetWrapperPlugin::ProcessConstructors(const Class &cls, CSharpClass &csCls) const {
  if (cls.IsProxyable()) {
    return;
  }
  bool added = false;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    if (!method.mFunction ||
        !method.mFunction->GetMatchingAnnotation(Annotations::Func, Annotations::Func_Constructor))
      continue;
    auto csDelegate =
        CSharpMethodHelper(mProject, cls, csCls, CSharpMethodType::WrappedClassDelegate)
            .GenerateMethod(method);
    csCls.mDelegates.push_back(csDelegate);

    auto csCtor =
        CSharpHelper::Get().CreateConstructor(mProject, cls, method, InteropType::ManagedToNative);
    GenerateConstructorWrapperCall(csCtor.mBody, cls, csCls, method, csCtor);
    csCls.mConstructors.push_back(std::move(csCtor));
    added = true;
  }
  if (added) {
    // add a default ctor
    csCls.mConstructors.emplace_back();
  }
}

void DotNetWrapperPlugin::ProcessMethods(const Class &cls, CSharpClass &csCls) const {
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    if (method.mFunction &&
        method.mFunction->GetMatchingAnnotation(Annotations::Func, Annotations::Func_Constructor))
      continue;
    auto csDelegate =
        CSharpMethodHelper(mProject, cls, csCls, CSharpMethodType::WrappedClassDelegate)
            .GenerateMethod(method);
    csCls.mDelegates.push_back(std::move(csDelegate));

    auto csMethod = CSharpHelper::Get().CreateMethod(mProject, cls, method, InteropType::Internal,
                                                     InteropType::Internal, false, true, false);
    csMethod.mStaticness = method.IsStatic(cls) ? Staticness::Static : Staticness::NotStatic;
    CSharpHelper::Get().GenerateWrapperCall(
        csMethod.mBody, mProject, InteropType::ManagedToNative, InteropType::NativeToManaged,
        CSharpHelper::Get().GetWrapperTargetInWrappedClass(csCls, Naming(), method.mName), csCls,
        method, csMethod, !method.IsStatic(cls), false);
    csCls.mMethods.push_back(std::move(csMethod));
  }
}

void DotNetWrapperPlugin::ProcessMethodPointers(const Class &cls, CSharpClass &csCls) const {
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    auto field =
        CSharpClassField(Naming().CSharpMethodPointerName(method.mName), CSharpType{"IntPtr"},
                         CSharpVisibility::Private, Staticness::Static, "IntPtr.Zero");
    csCls.mFields.push_back(std::move(field));
  }
}

void DotNetWrapperPlugin::ProcessProxyFields(const Class &cls, CSharpClass &csCls) const {
  for (auto &field: cls.mFields) {
    if (!field.mField || !ShouldProcess(field))
      continue;
    auto getter = GetGetter(cls, field);
    auto setter = GetSetter(cls, field);
    if (!getter && !setter)
      continue;
    auto &csField = csCls.mFields.emplace_back(
        Naming().FieldNameInCSharp(field.mField->mName),
        CSharpHelper::Get().ConvertType(field.mType, mProject, InteropType::Internal, false),
        CSharpVisibility::Public);
    if (getter) {
      csField.mGetter = CSharpMethodBase{};
      csField.mGetter->mBody.Add("{}();", getter->mName);
      if (auto csMethod = csCls.GetFirstMethod(getter->mName)) {
        csMethod->mVisibility = CSharpVisibility::Private;
      }
    }
    if (setter) {
      csField.mSetter = CSharpMethodBase{};
      csField.mSetter->mBody.Add("{}(value);", setter->mName);
      if (auto csMethod = csCls.GetFirstMethod(setter->mName)) {
        csMethod->mVisibility = CSharpVisibility::Private;
      }
    }
  }
}

void DotNetWrapperPlugin::ProcessProxy(const Class &cls, CSharpClass &csCls) const {
  ProcessProxyFields(cls, csCls);
  (void)cls;
  auto &ptrField =
      csCls.mFields.emplace_back(St::CSharpProxyObjectPointerFieldName, CSharpType{"IntPtr"},
                                 CSharpVisibility::Public, Staticness::NotStatic);
  ptrField.mGetter = CSharpMethodBase{};
  auto &ctor = csCls.mConstructors.emplace_back();
  ctor.mArguments.emplace_back("ptr", CSharpType{"IntPtr"});
  ctor.mBody.Add("{} = ptr;", St::CSharpProxyObjectPointerFieldName);
}

void DotNetWrapperPlugin::ProcessMirror(const Class &cls, CSharpClass &csCls) const {
  auto &innerStruct = csCls.mInnerClasses.emplace_back(St::CSharpMirroredStructStructName);
  innerStruct.mType = CSharpClassType::Struct;
  innerStruct.mAttributes.emplace_back("StructLayout(LayoutKind.Sequential)");
  for (auto &field: cls.mFields) {
    if (!ShouldProcess(field))
      continue;
    auto fieldName = field.mField ? St::Capitalize(field.mField->mName) : field.mName;
    auto fieldType =
        CSharpHelper::Get().ConvertType(field.mType, mProject, InteropType::ManagedToNative, false);
    innerStruct.mFields.emplace_back(fieldName, fieldType, CSharpVisibility::Public);
    auto &csField = csCls.mFields.emplace_back(fieldName, fieldType, CSharpVisibility::Public);
    csField.mDirectTo = std::format("{}.{}", St::CSharpMirroredStructFieldName, fieldName);
  }

  csCls.mFields.emplace_back(St::CSharpMirroredStructFieldName,
                             CSharpType{St::CSharpMirroredStructStructName},
                             CSharpVisibility::Public);
}

void DotNetWrapperPlugin::ProcessInitializer(const Class &cls, CSharpClass &csCls) const {
  auto csMethod =
      CSharpMethod{Naming().CSharpMethodDelegateName(cls.mName, St::CSharpInitializerMethodName),
                   CSharpType{"void"}, CSharpVisibility::Public, Staticness::Static};
  ProcessInitializerArguments(cls, csCls, csMethod);
  csCls.mDelegates.push_back(csMethod);

  csMethod.mName = St::CSharpInitializerMethodName;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    auto delegateName = Naming().CSharpMethodDelegateName(cls, method);
    csMethod.mBody.Add("{} = Marshal.GetFunctionPointerForDelegate({});",
                       Naming().CSharpMethodPointerName(method), St::Uncapitalize(delegateName));
  }
  csCls.mMethods.push_back(std::move(csMethod));
}

void DotNetWrapperPlugin::ProcessInitializerArguments(const Class &cls, const CSharpClass &csCls,
                                                      CSharpMethod &csMethod) const {
  (void)csCls;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    auto delegateName = Naming().CSharpMethodDelegateName(cls, method);
    csMethod.mArguments.emplace_back(St::Uncapitalize(delegateName), CSharpType{delegateName});
  }
}

bool DotNetWrapperPlugin::ShouldProcess(const Class &cls) const {
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::DotNetModule))
    return false;
  return true;
}

bool DotNetWrapperPlugin::ShouldProcess(const ClassMethod &method) const {
  if (!method.mExposeToScript)
    return false;
  if (!method.mFunction)
    return true;
  if (method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp) ||
      method.mFunction->GetAnnotation(Annotations::DotNetModule))
    return false;
  return true;
}

bool DotNetWrapperPlugin::ShouldProcess(const ClassConstructor &ctor) const {
  if (!ctor.mFunction)
    return false;
  if (ctor.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      ctor.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp) ||
      ctor.mFunction->GetAnnotation(Annotations::DotNetModule))
    return false;
  return true;
}

bool DotNetWrapperPlugin::ShouldProcess(const ClassField &field) const {
  if (!field.mField)
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp))
    return false;
  return true;
}

bool DotNetWrapperPlugin::IsStaticClass(const Class &cls) const {
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Interface))
    return false;
  for (auto &method: cls.mMethods) {
    if (ShouldProcess(method) && method.mStaticness != Staticness::Static)
      return false;
  }
  for (auto &field: cls.mFields) {
    if (ShouldProcess(field) && field.mStaticness != Staticness::Static)
      return false;
  }
  return true;
}

void DotNetWrapperPlugin::GenerateConstructorWrapperCall(CodeBlock &codeBlock, const Class &cls,
                                                         const CSharpClass &csCls,
                                                         const ClassMethod &method,
                                                         const CSharpMethodBase &csMethod) const {

  auto caller = CSharpHelper::Get().ConstructWrapperCall(
      csCls, mProject, InteropType::ManagedToNative,
      CSharpHelper::Get().GetWrapperTargetInWrappedClass(csCls, Naming(), method.mName), method,
      csMethod, false, false, false, false);
  if (cls.IsProxyable()) {
    codeBlock.Add("{} = {};", St::CSharpProxyObjectPointerFieldName, caller);
  } else {
    codeBlock.Add("{} = {};", St::CSharpMirroredStructFieldName, caller);
  }
}

const ClassMethod *DotNetWrapperPlugin::GetGetter(const Class &cls, const ClassField &field) const {
  if (!field.mField)
    return nullptr;
  auto attrib = field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Get);
  if (attrib && attrib->mValue.mName != Annotations::MethodOption_Custom)
    return nullptr;
  return cls.GetMethod(Naming().FieldGetterNameInCpp(field.mField->mName), Constness::Const);
}

const ClassMethod *DotNetWrapperPlugin::GetSetter(const Class &cls, const ClassField &field) const {
  if (!field.mField)
    return nullptr;
  auto attrib = field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Set);
  if (attrib && attrib->mValue.mName != Annotations::MethodOption_Custom)
    return nullptr;
  return cls.GetMethod(Naming().FieldSetterNameInCpp(field.mField->mName), Constness::NotConst);
}

} // namespace holgen
