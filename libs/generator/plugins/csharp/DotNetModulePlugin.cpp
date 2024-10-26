#include "DotNetModulePlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void DotNetModulePlugin::Run() {
  const Class *foundModule = nullptr;
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    THROW_IF(foundModule, "Found multiple dotnet module definitions: {} and {}",
             foundModule->mStruct->mDefinitionSource, cls.mStruct->mDefinitionSource);
    foundModule = &cls;
    Process(cls);
  }
}

void DotNetModulePlugin::Process(Class &cls) {
  cls.mClassType = ClassType::Struct;
  for (auto &header: {"hostfxr.h", "coreclr_delegates.h"}) {
    cls.mHeaderIncludes.AddLocalHeader(header);
  }

  cls.mFields.emplace_back(Naming().FieldNameInCpp("name"), Type{"std::string"},
                           Visibility::Public);

  std::vector<std::pair<const char *, const char *>> moduleFields {
    {"hostfxr_handle", "hostfxrHandle"}, {"load_assembly_fn", "hostfxrDelegate_load_assembly"}, {
      "get_function_pointer_fn", "hostfxrDelegate_get_function_pointer"
    }
  };
  for (auto &[type, field]: moduleFields) {
    cls.mFields.emplace_back(Naming().FieldNameInCpp(field), Type{type}, Visibility::Public,
                             Staticness::NotStatic, "nullptr");
  }
}
} // namespace holgen
