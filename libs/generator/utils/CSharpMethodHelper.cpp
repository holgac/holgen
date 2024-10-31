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
  AddAuxiliaryArguments(csMethod.mArguments, method.mReturnType,
                        St::CSharpAuxiliaryReturnValueArgName, true);
  GenerateMethodBody(method, csMethod);
  SetMethodProperties(method, csMethod);
  return csMethod;
}

CSharpConstructor CSharpMethodHelper::GenerateConstructor(const ClassMethod &method) {
  auto csCtor = CSharpConstructor{};
  PopulateArguments(csCtor.mArguments, method.mArguments);
  GenerateMethodBody(method, csCtor);
  return csCtor;
}

std::string CSharpMethodHelper::ConstructWrapperCall(const std::string &methodToCall,
                                                     const ClassMethod &method,
                                                     const CSharpMethodBase &csMethod) {
  return std::format("{}({})", methodToCall, ConstructMethodArguments(method, csMethod));
}

void CSharpMethodHelper::SetMethodProperties(const ClassMethod &method, CSharpMethod &csMethod) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
    break;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    csMethod.mStaticness = method.IsStatic(mClass) ? Staticness::Static : Staticness::NotStatic;
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
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
      return CSharpType{cls->mName};
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
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
      {
        auto out = ConvertReturnType(type.mTemplateParameters.front());
        ++out.mArrayDepth;
        return out;
      }
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      return CSharpType{"IntPtr"};
    }
  }
  THROW("Unhandled type: {}", type.ToString(true, true));
}

CSharpType CSharpMethodHelper::ConvertArgumentType(const Type &type) {
  if (type.mName == "char" && type.mType == PassByType::Pointer &&
      type.mConstness == Constness::Const) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
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

  if (type.mName == "void" && type.mType == PassByType::Pointer) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
      break;
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      THROW("{} is not a valid type for this method type!", type.ToString(true));
    }
    return CSharpType{"IntPtr"};
  }

  auto it = CSharpHelper::Get().CppTypeToCSharpType.find(type.mName);
  if (it != CSharpHelper::Get().CppTypeToCSharpType.end()) {
    THROW_IF(type.mType == PassByType::Pointer,
             "Primitive pointer return types are not supported!");
    return CSharpType{it->second};
  }
  if (auto cls = mProject.GetClass(type.mName)) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
      return CSharpType{cls->mName};
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
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
      {
        auto out = ConvertArgumentType(type.mTemplateParameters.front());
        ++out.mArrayDepth;
        return out;
      }
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
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return rawName;
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldAddThisArgument(const ClassMethod &method) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
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
                                       size_t sizeArgIndex) {
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
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return false;
  }
  THROW("Unexpected method type!");
}

void CSharpMethodHelper::AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments,
                                               const Type &type, const std::string &argPrefix,
                                               bool isReturnValue) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return;
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassDelegate:
    break;
  }
  if (CSharpHelper::Get().NeedsSizeArgument(type)) {
    auto &arg = arguments.emplace_back(
        std::format("{}{}", argPrefix, St::CSharpAuxiliarySizeSuffix), CSharpType{"ulong"});
    if (isReturnValue) {
      arg.mType.mType = CSharpPassByType::Out;
    }
  }
  if (isReturnValue && ShouldHaveDeleter(type)) {
    auto &deleterArg =
        arguments.emplace_back(St::DeferredDeleterArgumentName, CSharpType{"IntPtr"});
    deleterArg.mType.mType = CSharpPassByType::Out;
  }
}

bool CSharpMethodHelper::ShouldHaveDeleter(const Type &returnType) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return false;
  case CSharpMethodType::WrappedClassDelegate:
    return (returnType.mName == "std::array" || returnType.mName == "std::vector" ||
            returnType.mName == "std::span");
  }
  THROW("Unexpected method type!");
}

std::string CSharpMethodHelper::ConstructMethodArguments(const ClassMethod &method,
                                                         const CSharpMethodBase &csMethod) {
  std::stringstream ss;
  bool isFirst = true;

  // if (addThisArgument) {
  //   if (interopType != InteropType::NativeToManaged && cls.mClass && !cls.mClass->IsProxyable())
  //   {
  //     ss << "ref ";
  //   }
  //
  //   ss << VariableRepresentation(CSharpType{cls.mName}, "this", project, interopType);
  //   isFirst = false;
  // }


  auto csIt = csMethod.mArguments.begin(), csEnd = csMethod.mArguments.end();
  auto it = method.mArguments.begin(), end = method.mArguments.end();
  for (; csIt != csEnd; ++it, ++csIt) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    THROW_IF(it == end || it->mName != csIt->mName, "Argument order got messed up!");
    // if (auto argClass = mProject.GetClass(it->mType.mName)) {
    //   if (interopType != InteropType::NativeToManaged && !argClass->IsProxyable()) {
    //     ss << "ref ";
    //   }
    // }
    ss << VariableRepresentation(csIt->mType, csIt->mName);
    // if (!ignoreAuxiliaries) {
    //   ss << StringifyPassedExtraArguments(it->mType, csIt->mName, interopType);
    // }
  }
  // if (hasSizeArg && !ignoreAuxiliaries) {
  //   if (!isFirst)
  //     ss << ", ";
  //   if (interopType == InteropType::NativeToManaged)
  //     ss << "out ";
  //   else
  //     ss << "out var ";
  //   ss << St::CSharpAuxiliaryReturnValueArgName << St::CSharpAuxiliarySizeSuffix;
  //   isFirst = false;
  // }

  // if (hasDeleterArg && !ignoreAuxiliaries) {
  //   if (!isFirst) {
  //     ss << ", ";
  //   }
  //   ss << "out var " << St::DeferredDeleterArgumentName;
  // }
  return ss.str();
}

std::string CSharpMethodHelper::VariableRepresentation(const CSharpType &type,
                                                       const std::string &variableName) {
  auto cls = mProject.GetClass(type.mName);
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    break;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    if (cls && cls->IsProxyable()) {
      return std::format("{}{}.{}", type.mType, variableName,
                         St::CSharpProxyObjectPointerFieldName);
    } else if (cls) {
      return std::format("{}{}.{}", type.mType, variableName, St::CSharpMirroredStructFieldName);
    } else {
      return std::format("{}{}", type.mType, variableName);
    }
  }
  THROW("Unexpected VariableRepresentation operation for the given method type!");
}

std::string
    CSharpMethodHelper::GetWrapperTargetInWrappedClass(const std::string &wrappedMethodName) const {
  return std::format("Marshal.GetDelegateForFunctionPointer<{}>({})",
                     mNaming.CSharpMethodDelegateName(mClass.mName, wrappedMethodName),
                     mNaming.CSharpMethodPointerName(wrappedMethodName));
}

void CSharpMethodHelper::GenerateMethodBody(const ClassMethod &method, CSharpMethodBase &csMethod) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    return;
  case CSharpMethodType::WrappedClassCallerMethod:
    return;
  case CSharpMethodType::WrappedClassCallerConstructor:
    GenerateMethodBodyForConstructor(method, csMethod);
  }
}

void CSharpMethodHelper::GenerateMethodBodyForConstructor(const ClassMethod &method,
                                                          CSharpMethodBase &csMethod) {
  auto caller =
      ConstructWrapperCall(GetWrapperTargetInWrappedClass(method.mName), method, csMethod);
  if (mClass.IsProxyable()) {
    csMethod.mBody.Add("{} = {};", St::CSharpProxyObjectPointerFieldName, caller);
  } else {
    csMethod.mBody.Add("{} = {};", St::CSharpMirroredStructFieldName, caller);
  }
}

} // namespace holgen
