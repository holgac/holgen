#include "DotNetModuleInterfacePlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/utils/CSharpHelper.h"

namespace holgen {
void DotNetModuleInterfacePlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    Process(cls);
  }
}

void DotNetModuleInterfacePlugin::Process(Class &cls) const {
  auto &csCls = mProject.mCSharpClasses.emplace_back("I" + cls.mName, &cls);
  csCls.mType = CSharpClassType::Interface;
  csCls.mUsingDirectives.insert("System.Runtime.InteropServices");
  for (auto &method: cls.mMethods) {
    auto csMethod =
        CSharpHelper::Get().CreateMethod(mProject, cls, method, InteropType::NativeToManaged,
                                         InteropType::NativeToManaged, false, false, true);
    auto oldName = csMethod.mName;
    csMethod.mName = Naming().CSharpMethodDelegateName(cls.mName, method.mName),
    csCls.mDelegates.push_back(csMethod);
    csMethod.mName = oldName;
    csMethod.mVirtuality = Virtuality::PureVirtual;
    csMethod.mStaticness = Staticness::Static;
    csCls.mMethods.push_back(csMethod);
  }
}
} // namespace holgen
