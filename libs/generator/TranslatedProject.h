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
  [[nodiscard]] std::vector<std::pair<Class *, const EnumEntryDefinition *>>
      GetVariantClassesOfEnum(const std::string &enumName);
};

} // namespace holgen

namespace std {
template <>
struct formatter<holgen::Visibility> : formatter<std::string> {
  auto format(const holgen::Visibility &visibility, format_context &ctx) const {
    switch (visibility) {
    case holgen::Visibility::Public:
      return std::format_to(ctx.out(), "public");
    case holgen::Visibility::Protected:
      return std::format_to(ctx.out(), "protected");
    case holgen::Visibility::Private:
      return std::format_to(ctx.out(), "private");
    }
    THROW("Unexpected visibility: {}", uint32_t(visibility));
  }
};

template <>
struct formatter<holgen::CSharpClassType> : formatter<std::string> {
  auto format(const holgen::CSharpClassType &type, format_context &ctx) const {
    switch (type) {
    case holgen::CSharpClassType::Class:
      return std::format_to(ctx.out(), "class");
    case holgen::CSharpClassType::Struct:
      return std::format_to(ctx.out(), "struct");
    case holgen::CSharpClassType::Interface:
      return std::format_to(ctx.out(), "interface");
    }
    THROW("Unexpected C# class type: {}", uint32_t(type));
  }
};
} // namespace std
