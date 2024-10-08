#include "ClassMetadataPlugin.h"
#include "core/St.h"

namespace holgen {
void ClassMetadataPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    Process(cls);
  }
}

void ClassMetadataPlugin::Process(Class &cls) {
  auto field = ClassField{St::ClassName, Type{"char", PassByType::Pointer, Constness::Const},
                          Visibility::Public, Staticness::Static};
  field.mType.mConstexprness = Constexprness::Constexpr;
  field.mDefaultValue = std::format("\"{}\"", cls.mName);
  Validate().NewField(cls, field);
  cls.mFields.push_back(std::move(field));
}
}
