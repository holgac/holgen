#include "ClassRuleOfFivePlugin.h"

namespace holgen {
void ClassRuleOfFivePlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    ProcessClass(cls);
  }
}

void ClassRuleOfFivePlugin::ProcessClass(Class &cls) {
  auto moveCtor = cls.GetMoveConstructor();
  auto moveOp = cls.GetMoveAssignment();
  auto copyCtor = cls.GetCopyConstructor();
  auto copyOp = cls.GetCopyAssignment();
  int count = !!moveCtor + !!moveOp + !!copyCtor + !!copyOp + !cls.mDestructor.IsEmpty();

  if (count == 0) {
    return;
  }

  if (!cls.GetDefaultConstructor()) {
    auto ctor = ClassConstructor{};
    ctor.mDefaultDelete = DefaultDelete::Default;
    cls.mConstructors.push_back(std::move(ctor));
  }

  if (count == 5) {
    return;
  }

  if (!moveCtor) {
    auto ctor = ClassConstructor{};
    ctor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    ctor.mDefaultDelete = DefaultDelete::Default;
    cls.mConstructors.push_back(std::move(ctor));
  }

  if (!copyCtor) {
    auto ctor = ClassConstructor{};
    ctor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    ctor.mDefaultDelete = DefaultDelete::Default;
    cls.mConstructors.push_back(std::move(ctor));
  }

  if (!moveOp) {
    auto op = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference}, Visibility::Public,
                          Constness::NotConst};
    op.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    op.mDefaultDelete = DefaultDelete::Default;
    cls.mMethods.push_back(std::move(op));
  }

  if (!copyOp) {
    auto op = ClassMethod{"operator=", Type{cls.mName, PassByType::Reference}, Visibility::Public,
                          Constness::NotConst};
    op.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    op.mDefaultDelete = DefaultDelete::Default;
    cls.mMethods.push_back(std::move(op));
  }

  if (cls.mDestructor.IsEmpty()) {
    cls.mDestructor.mDefaultDelete = DefaultDelete::Default;
  }
}
} // namespace holgen
