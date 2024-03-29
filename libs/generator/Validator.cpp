#include "Validator.h"
#include <set>
#include <string>
#include "TypeInfo.h"
#include "core/St.h"
#include "core/Exception.h"
#include "TranslatedProject.h"

namespace holgen {
  namespace {
    std::set<std::string> ReservedKeywords{
        "struct",
        "class",
        "namespace",
        "template",
    };
    std::set<std::string> CustomTypes{
        "Ref",
    };
    std::string ToString(const Class& cls) {
      if (cls.mStruct)
        return std::format("{} ({})", cls.mName, cls.mStruct->mDefinitionSource);
      else if (cls.mEnum)
        return std::format("{} ({})", cls.mName, cls.mEnum->mDefinitionSource);
      else
        return std::format("{}", cls.mName);
    }
  }

  Validator::Validator(TranslatedProject &project) : mProject(project) {}

  void Validator::NewClass(const Class &cls) const {
    THROW_IF(ReservedKeywords.contains(cls.mName), "{} is a reserved keyword", ToString(cls));
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(cls.mName), "{} is a reserved keyword", ToString(cls));
    auto dup = mProject.GetClass(cls.mName);
    THROW_IF(dup, "Duplicate class: {} and {}", ToString(*dup), ToString(cls));
  }
}
