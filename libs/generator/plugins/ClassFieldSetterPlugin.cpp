#include "ClassFieldSetterPlugin.h"
#include "core/St.h"
#include "../NamingConvention.h"

namespace holgen {
  void ClassFieldSetterPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      for (auto &field: cls.mFields) {
        if (!field.mField)
          continue;
        auto method = ClassMethod{
            Naming().FieldSetterNameInCpp(*field.mField), Type{"void"},
            Visibility::Public, Constness::NotConst};
        auto &arg = method.mArguments.emplace_back("val", field.mType);
        if (field.mType.mType == PassByType::Pointer)
          arg.mType.mType = PassByType::Pointer;
        else
          arg.mType.PreventCopying();
        method.mBody.Add("{} = val;", field.mName);
        Validate().NewMethod(cls, method);
        cls.mMethods.push_back(std::move(method));
      }
    }
  }
}
