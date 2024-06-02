#include "ClassNonCopyablePlugin.h"

namespace holgen {

void ClassNonCopyablePlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    ProcessStructDefinition(cls, *cls.mStruct);
  }
}

void ClassNonCopyablePlugin::ProcessStructDefinition(Class &cls,
                                                     const StructDefinition &structDefinition) {
  // TODO: validate: a nonCopyable field makes the class nonCopyable as well

  if (structDefinition.GetMatchingAttribute(Annotations::Struct, Annotations::Struct_NonCopyable)) {
    // holgen structs don't have constructors, so doing this here makes sense.
    // Need to add default empty ctor at a later stage if this changes.
    {
      auto ctor = ClassConstructor{};
      ctor.mDefaultDelete = DefaultDelete::Default;
      cls.mConstructors.push_back(std::move(ctor));
    }

    {
      auto ctor = ClassConstructor{};
      ctor.mArguments.emplace_back("rhs",
                                   Type{cls.mName, PassByType::MoveReference, Constness::NotConst});
      ctor.mDefaultDelete = DefaultDelete::Default;
      cls.mConstructors.push_back(std::move(ctor));
    }

    {
      auto ctor = ClassConstructor{};
      ctor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
      ctor.mDefaultDelete = DefaultDelete::Delete;
      cls.mConstructors.push_back(std::move(ctor));
    }
    // TODO: validate ctors

    {
      auto method = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference},
                                Visibility::Public, Constness::NotConst};
      method.mDefaultDelete = DefaultDelete::Default;
      method.mArguments.emplace_back(
          "rhs", Type{cls.mName, PassByType::MoveReference, Constness::NotConst});
      Validate().NewMethod(cls, method);
      cls.mMethods.push_back(std::move(method));
    }

    {
      auto method = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference},
                                Visibility::Public, Constness::NotConst};
      method.mDefaultDelete = DefaultDelete::Delete;
      method.mArguments.emplace_back("rhs",
                                     Type{cls.mName, PassByType::Reference, Constness::Const});
      Validate().NewMethod(cls, method);
      cls.mMethods.push_back(std::move(method));
    }
  }
}
} // namespace holgen
