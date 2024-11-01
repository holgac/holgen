#include "DotNetDeferredDeleterPlugin.h"

#include "generator/utils/CSharpMethodHelper.h"
#include "core/St.h"

namespace holgen {
void DotNetDeferredDeleterPlugin::Run() {
  auto csCls = mProject.GetCSharpClass(St::DeferredDeleter);
  auto cls = mProject.GetClass(St::DeferredDeleter);
  if (!csCls)
    return;
  THROW_IF(!cls, "DeferredDeleter plugin did not run")
  GeneratePerformManaged(*cls, *csCls);
  GeneratePerformManagedArray(*cls, *csCls);
}

void DotNetDeferredDeleterPlugin::GeneratePerformManaged(const Class &cls, CSharpClass &csCls) {

  auto method = cls.GetMethod(St::DeferredDeleterPerformManaged, Constness::NotConst);
  THROW_IF(!method, "DeferredDeleter plugin did not run")
  csCls.mDelegates.push_back(CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                                CSharpMethodType::InterfaceClassMethodDelegate)
                                 .GenerateMethod(*method));

  auto csMethod = CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                     CSharpMethodType::InterfaceClassMethodCaller)
                      .GenerateMethod(*method);
  csMethod.mName = method->mName;
  csMethod.mBody = {};
  csMethod.mVirtuality = Virtuality::NotVirtual;
  csMethod.mStaticness = Staticness::Static;
  csMethod.mBody.Add("Marshal.FreeHGlobal({});", csMethod.mArguments.front().mName);
  csCls.mMethods.push_back(std::move(csMethod));
}

void DotNetDeferredDeleterPlugin::GeneratePerformManagedArray(const Class &cls,
                                                              CSharpClass &csCls) {
  auto method = cls.GetMethod(St::DeferredDeleterPerformManagedArray, Constness::NotConst);
  THROW_IF(!method, "DeferredDeleter plugin did not run")
  csCls.mDelegates.push_back(CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                                CSharpMethodType::InterfaceClassMethodDelegate)
                                 .GenerateMethod(*method));

  auto csMethod = CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                     CSharpMethodType::InterfaceClassMethodCaller)
                      .GenerateMethod(*method);
  csMethod.mName = method->mName;
  csMethod.mBody = {};
  csMethod.mVirtuality = Virtuality::NotVirtual;
  csMethod.mStaticness = Staticness::Static;
  csMethod.mBody.Add("for (int i = 0; i < (int){}; ++i)", csMethod.mArguments.back().mName);
  csMethod.mBody.Add("{{");
  csMethod.mBody.Indent(1);
  csMethod.mBody.Add("var data = Marshal.ReadIntPtr({}, i * IntPtr.Size);",
                     csMethod.mArguments.front().mName);
  csMethod.mBody.Add("Marshal.FreeHGlobal(data);");
  csMethod.mBody.Indent(-1);
  csMethod.mBody.Add("}}");
  csMethod.mBody.Add("Marshal.FreeHGlobal({});", csMethod.mArguments.front().mName);
  csCls.mMethods.push_back(std::move(csMethod));
}
} // namespace holgen
