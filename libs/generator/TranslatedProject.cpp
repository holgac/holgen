#include "TranslatedProject.h"
#include "holgen.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

TranslatedProject::TranslatedProject(const ProjectDefinition &projectDefinition) :
    mProject(projectDefinition), mDependencyGraph(mProject) {}

GEN_GETTER_BY_NAME(TranslatedProject, Class, GetClass, mClasses)

GEN_GETTER_BY_NAME_NONCONST(TranslatedProject, Class, GetClass, mClasses);

std::vector<std::pair<Class *, const EnumEntryDefinition *>>
    TranslatedProject::GetVariantClassesOfEnum(const std::string &enumName) {
  std::vector<std::pair<Class *, const EnumEntryDefinition *>> result;
  auto &enumDefinition = *mProject.GetEnum(enumName);
  for (auto &cls: mClasses) {
    if (!cls.mStruct || cls.mStruct->mIsMixin)
      continue;
    if (!cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Enum,
                                           enumName))
      continue;
    auto entry = cls.mStruct->GetMatchingAttribute(Annotations::Variant, Annotations::Variant_Entry)
                     ->mValue.mName;
    auto enumEntry = enumDefinition.GetEnumEntry(entry);
    THROW_IF(!enumEntry, "Entry {} of enum {} ({}) referenced by {} ({}) does not exist!", entry,
             enumDefinition.mName, enumDefinition.mDefinitionSource, cls.mName,
             cls.mStruct->mDefinitionSource);
    result.emplace_back(&cls, enumDefinition.GetEnumEntry(entry));
    ;
  }
  std::sort(result.begin(), result.end(),
            [](const auto &lhs, const auto &rhs) { return lhs.first->mName < rhs.first->mName; });
  return result;
}
} // namespace holgen
