#pragma once

#include <list>
#include <utility>

#include "CodeBlock.h"
#include "utils/HeaderContainer.h"
#include "TypeInfo.h"
#include "parser/DependencyGraph.h"
#include "core/Exception.h"
#include "types/CppTypes.h"
#include "types/CSharpTypes.h"

namespace holgen {

struct TranslatedProject {
public:
  explicit TranslatedProject(const ProjectDefinition &projectDefinition);
  const ProjectDefinition &mProject;
  const DependencyGraph mDependencyGraph;
  std::list<Class> mClasses;
  std::list<CSharpClass> mCSharpClasses;
  [[nodiscard]] Class *GetClass(const std::string &name);
  [[nodiscard]] const Class *GetClass(const std::string &name) const;
  [[nodiscard]] CSharpClass *GetCSharpClass(const std::string &name);
  [[nodiscard]] const CSharpClass *GetCSharpClass(const std::string &name) const;
  [[nodiscard]] std::vector<std::pair<Class *, const EnumEntryDefinition *>>
      GetVariantClassesOfEnum(const std::string &enumName);
};

} // namespace holgen
