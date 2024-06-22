#include "LuaFieldsPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/StringSwitcher.h"
#include "core/Exception.h"

namespace holgen {
void LuaFieldsPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct)
      ProcessStruct(cls);
  }
}

void LuaFieldsPlugin::ProcessRegistryDataField(Class &cls, const FieldDefinition &fieldDefinition) {
  // TODO: remove the line below after making LuaPlugin include lua.hpp in the header
  cls.mHeaderIncludes.AddLibHeader("lua.hpp");
  auto field = ClassField{Naming().FieldNameInCpp(fieldDefinition.mName), Type{"int"}};
  field.mDefaultValue = "LUA_NOREF";
  field.mField = &fieldDefinition;

  Validate().NewField(cls, field);
  cls.mFields.emplace_back(std::move(field));
}

void LuaFieldsPlugin::ProcessStruct(Class &cls) {
  if (cls.mStruct->GetAnnotation(Annotations::NoLua))
    return;
  for (auto &field: cls.mStruct->mFields) {
    if (field.mType.mName == St::Lua_RegistryData) {
      ProcessRegistryDataField(cls, field);
    }
  }
}
} // namespace holgen
