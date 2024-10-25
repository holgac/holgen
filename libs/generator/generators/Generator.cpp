#include "Generator.h"
namespace holgen {

std::string Generator::GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                                     bool isInHeader, bool isInsideClass) const {
  std::stringstream ss;
  if (method.mConstexprness == Constexprness::Constexpr)
    ss << "constexpr ";
  if (method.mStaticness == Staticness::Static && isInsideClass && isInHeader)
    ss << "static ";
  if (isInHeader && isInsideClass && method.mVirtuality != Virtuality::NotVirtual) {
    ss << "virtual ";
  }
  // TODO: this doesn't work if type is const
  if (!isInHeader && cls.GetUsing(method.mReturnType.mName))
    ss << cls.mName << "::";
  ss << method.mReturnType.ToString(false);
  if (!isInHeader || !isInsideClass)
    ss << cls.mName << "::";
  ss << method.mName << "(";
  size_t idx = 0;
  for (auto &arg: method.mArguments) {
    if (idx != 0)
      ss << ", ";
    ss << arg.mType.ToString(false) << arg.mName;
    if (isInHeader && arg.mDefaultValue.has_value())
      ss << " = " << *arg.mDefaultValue;
    ++idx;
  }
  ss << ")";
  if (method.mConstness == Constness::Const)
    ss << " const";
  if (method.mNoexceptness == Noexceptness::Noexcept)
    ss << " noexcept";
  return ss.str();
}

std::string Generator::GenerateFunctionSignature(const Class &cls, const ClassConstructor &ctor,
                                                     bool isInHeader, bool isInsideClass) const {
  std::stringstream ss;
  if (isInHeader && isInsideClass && ctor.mExplicitness == Explicitness::Explicit)
    ss << "explicit ";
  if (!isInsideClass)
    ss << cls.mName << "::";
  ss << cls.mName << "(";
  bool isFirst = true;
  for (auto &arg: ctor.mArguments) {
    if (isFirst)
      isFirst = false;
    else
      ss << ", ";
    // TODO: this doesn't work if type is const
    if (cls.GetUsing(arg.mType.mName) && !isInsideClass)
      ss << cls.mName << "::";
    ss << arg.mType.ToString(false) << arg.mName;
    if (isInsideClass && arg.mDefaultValue.has_value()) {
      ss << " = " << *arg.mDefaultValue;
    }
  }
  ss << ")";
  if (ctor.mNoexceptness == Noexceptness::Noexcept)
    ss << " noexcept";
  return ss.str();
}

std::string Generator::GenerateFunctionSignature(const CFunction &func, bool isInHeader) const {
  std::stringstream ss;
  if (isInHeader) {
    ss << "HOLGEN_EXPORT ";
  }
  ss << func.mReturnType.ToFullyQualifiedString(mTranslatedProject);

  ss << func.mName << "(";
  size_t idx = 0;
  for (auto &arg: func.mArguments) {
    if (idx != 0)
      ss << ", ";
    ss << arg.mType.ToFullyQualifiedString(mTranslatedProject) << arg.mName;
    ++idx;
  }
  ss << ")";
  return ss.str();
}

}
