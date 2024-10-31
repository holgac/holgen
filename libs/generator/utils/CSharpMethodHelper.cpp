#include "CSharpMethodHelper.h"
#include "core/St.h"
#include "generator/types/CppTypes.h"

namespace holgen {
CSharpMethod CSharpMethodHelper::GenerateMethod(const ClassMethod &method) {
  auto csMethod = CSharpMethod{GetMethodName(method.mName), ConvertReturnType(method.mReturnType)};
  if (ShouldAddThisArgument(method)) {
    auto &arg = csMethod.mArguments.emplace_back(St::CSharpHolgenObjectArg,
                                                 ConvertArgumentType(Type{mClass.mName}));
    if (!mClass.IsProxyable())
      arg.mType.mType = CSharpPassByType::Ref;
  }
  PopulateArguments(csMethod.mArguments, method.mArguments);
  // AddAttributes(csMethod.mAttributes, method.mReturnType, csMethod.mReturnType, true,
  //               csMethod.mArguments.size());
  AddAuxiliaryArguments(csMethod.mArguments, method.mReturnType,
                        St::CSharpAuxiliaryReturnValueArgName, true);
  PostProcess(method, csMethod);
  return csMethod;
}

void CSharpMethodHelper::PostProcess(const ClassMethod &method, CSharpMethod &csMethod) {
  (void)method;
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
    break;
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    csMethod.mVirtuality = Virtuality::PureVirtual;
    csMethod.mStaticness = Staticness::Static;
    break;
  }
}

CSharpType CSharpMethodHelper::ConvertReturnType(const Type &type) {
  auto it = CSharpHelper::Get().CppTypeToCSharpType.find(type.mName);
  if (it != CSharpHelper::Get().CppTypeToCSharpType.end()) {
    THROW_IF(type.mType == PassByType::Pointer,
             "Primitive pointer return types are not supported!");
    return CSharpType{it->second};
  }
  if (auto cls = mProject.GetClass(type.mName)) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      if (cls->IsProxyable()) {
        return CSharpType{"IntPtr"};
      } else {
        return CSharpType{std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName)};
      }
    }
  }
  if (type.mName == "std::span" || type.mName == "std::array" || type.mName == "std::vector") {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      return CSharpType{"IntPtr"};
    }
  }
  THROW("Unhandled type: {}", type.ToString(true, true));
  ;
}

CSharpType CSharpMethodHelper::ConvertArgumentType(const Type &type) {
  if (type.mName == "char" && type.mType == PassByType::Pointer &&
      type.mConstness == Constness::Const) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
      THROW("{} is not a valid type for this method type!", type.ToString(true));
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      break;
    }
    auto out = CSharpType{"string"};
    out.mArrayDepth = type.mPointerDepth;
    THROW_IF(out.mArrayDepth > 1, "{} cannot be represented in c#: pointer level too deep!",
             type.ToString(true));
    return out;
  }

  auto it = CSharpHelper::Get().CppTypeToCSharpType.find(type.mName);
  if (it != CSharpHelper::Get().CppTypeToCSharpType.end()) {
    THROW_IF(type.mType == PassByType::Pointer,
             "Primitive pointer return types are not supported!");
    return CSharpType{it->second};
  }
  if (auto cls = mProject.GetClass(type.mName)) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      if (cls->IsProxyable()) {
        return CSharpType{"IntPtr"};
      } else {
        return CSharpType{std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName)};
      }
    }
  }
  if (type.mName == "std::span" || type.mName == "std::array" || type.mName == "std::vector") {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      {
        auto out = ConvertArgumentType(type.mTemplateParameters.front());
        ++out.mArrayDepth;
        return out;
      }
    }
    // return CSharpType{"IntPtr"};
  }
  THROW("Unhandled type: {}", type.ToString(true, true));
}

std::string CSharpMethodHelper::GetMethodName(const std::string &rawName) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
    return std::format("{}{}{}", mClass.mName, rawName, St::CSharpDelegateSuffix);
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    return rawName;
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldAddThisArgument(const ClassMethod &method) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    return false;
  case CSharpMethodType::WrappedClassDelegate:
    return !method.IsStatic(mClass);
  }
  THROW("Unexpected method type!");
}

void CSharpMethodHelper::PopulateArguments(std::list<CSharpMethodArgument> &out,
                                           const std::list<MethodArgument> &arguments) {
  for (auto &arg: arguments) {
    auto &csArg = out.emplace_back(arg.mName, ConvertArgumentType(arg.mType), arg.mDefaultValue);
    AddAttributes(csArg.mAttributes, arg.mType, csArg.mType, false, out.size());
    if (ShouldUseRefArgument(arg.mType, csArg.mType))
      csArg.mType.mType = CSharpPassByType::Ref;
    AddAuxiliaryArguments(out, arg.mType, csArg.mName, false);
  }
}

void CSharpMethodHelper::AddAttributes(std::list<std::string> &attributes, const Type &type,
                                       const CSharpType &csType, bool isReturnType,
                                       size_t sizeArgIndex) const {
  std::string prefix = isReturnType ? "return: " : "";
  if (type.mName == "char" && type.mType == PassByType::Pointer) {
    if (csType.mArrayDepth == 0) {
      attributes.emplace_back(std::format("{}MarshalAs(UnmanagedType.LPStr)", prefix));
    } else {
      attributes.emplace_back(std::format(
          "{}MarshalAs(UnmanagedType.LPArray, ArraySubType=UnmanagedType.LPStr, SizeParamIndex={})",
          prefix, sizeArgIndex));
    }
  }
  if (type.mName == "std::array") {
    attributes.emplace_back(std::format("{}MarshalAs(UnmanagedType.LPArray, SizeConst={})", prefix,
                                        type.mTemplateParameters.back().mName));
  } else if (type.mName == "std::vector" || type.mName == "std::span") {
    attributes.emplace_back(
        std::format("{}MarshalAs(UnmanagedType.LPArray, SizeParamIndex={})", prefix, sizeArgIndex));
  }
}

bool CSharpMethodHelper::ShouldUseRefArgument(const Type &type, const CSharpType &csType) {
  (void)csType;
  auto cls = mProject.GetClass(type.mName);
  if (!cls || cls->IsProxyable())
    return false;
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    return true;
  }
  THROW("Unexpected method type!");
}

void CSharpMethodHelper::AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments,
                                               const Type &type, const std::string &argPrefix,
                                               bool isReturnValue) const {
  if (CSharpHelper::Get().NeedsSizeArgument(type)) {
    auto &arg = arguments.emplace_back(
        std::format("{}{}", argPrefix, St::CSharpAuxiliarySizeSuffix), CSharpType{"ulong"});
    if (isReturnValue) {
      arg.mType.mType = CSharpPassByType::Out;
    }
  }
  if (isReturnValue && CSharpHelper::Get().NeedsDeleter(type)) {
    auto &deleterArg =
        arguments.emplace_back(St::DeferredDeleterArgumentName, CSharpType{"IntPtr"});
    deleterArg.mType.mType = CSharpPassByType::Out;
  }
}

bool CSharpMethodHelper::ShouldHaveDeleter(const Type &returnType) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    return (returnType.mName == "std::array" || returnType.mName == "std::vector" ||
            returnType.mName == "std::span");
  }
  THROW("Unexpected method type!");
}

} // namespace holgen
