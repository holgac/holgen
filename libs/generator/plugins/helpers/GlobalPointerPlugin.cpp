#include "GlobalPointerPlugin.h"
#include "core/St.h"

namespace holgen {
  void GlobalPointerPlugin::Run() {
    auto &cls = mProject.mClasses.emplace_back(St::GlobalPointer);
    auto &clsTemplate = cls.mTemplateParameters.emplace_back();
    clsTemplate.mType = "typename";
    clsTemplate.mName = "T";

    cls.mFields.emplace_back(
        "mInstance", Type{"T", PassByType::Pointer}, Visibility::Private,
        Staticness::Static, "nullptr");

    {
      auto &func = cls.mMethods.emplace_back(
          St::GlobalPointer_GetInstance, Type{"T", PassByType::Pointer}, Visibility::Public, Constness::NotConst,
          Staticness::Static);
      // TODO: move these strings to the plugins that generate them
      func.mBody.Add("return mInstance;");
    }

    {
      auto &func = cls.mMethods.emplace_back(
          St::GlobalPointer_SetInstance, Type{"void"}, Visibility::Public, Constness::NotConst, Staticness::Static);
      func.mArguments.emplace_back("ptr", Type{"T", PassByType::Pointer});
      // the users handle storage and deletion?
      func.mBody.Add("mInstance = ptr;");
    }
  }
}
