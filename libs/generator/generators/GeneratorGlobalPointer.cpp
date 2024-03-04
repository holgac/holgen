#include "GeneratorGlobalPointer.h"
#include "core/St.h"

namespace holgen {
  void GeneratorGlobalPointer::GenerateHelpers() {
    auto &cls = mTranslatedProject.mClasses.emplace_back();
    cls.mName = St::GlobalPointer;
    auto &clsTemplate = cls.mTemplateParameters.emplace_back();
    clsTemplate.mType = "typename";
    clsTemplate.mName = "T";

    {
      auto &field = cls.mFields.emplace_back();
      field.mType.mName = "T";
      field.mType.mType = PassByType::Pointer;
      field.mName = "mInstance";
      field.mVisibility = Visibility::Private;
      field.mIsStatic = true;
      field.mDefaultValue = "nullptr";
    }

    {
      auto &func = cls.mMethods.emplace_back();
      func.mName = "GetInstance";
      func.mIsStatic = true;
      func.mIsConst = false;
      func.mReturnType.mName = "T";
      func.mReturnType.mType = PassByType::Pointer;
      func.mBody.Add("return mInstance;");
    }

    {
      auto &func = cls.mMethods.emplace_back();
      func.mName = "SetInstance";
      func.mIsStatic = true;
      func.mIsConst = false;
      func.mIsConst = false;
      auto &arg = func.mArguments.emplace_back();
      arg.mType.mName = "T";
      arg.mType.mType = PassByType::Pointer;
      arg.mName = "ptr";
      // the user handle storage and deletion?
      func.mBody.Add("mInstance = ptr;");
    }
  }
}
