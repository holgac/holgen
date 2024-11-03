#include "CSharpMethodHelper.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "generator/types/CppTypes.h"

namespace holgen {
CSharpMethod CSharpMethodHelper::GenerateMethod(const ClassMethod &method) {
  auto csMethod = CSharpMethod{GetMethodName(method.mName), ConvertReturnType(method.mReturnType)};
  if (ShouldAddThisArgument(method)) {
    auto &arg = csMethod.mArguments.emplace_back(St::CSharpHolgenObjectArg,
                                                 ConvertArgumentType(Type{mClass.mName}));
    if (!mClass.IsProxyable() && !mClass.mEnum)
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

std::string CSharpMethodHelper::PassAuxiliaryArguments(const Type &type,
                                                       const std::string &argPrefix) {
  if (type.mName == "std::vector" || type.mName == "std::span") {
    return std::format(", (ulong){}.Length", argPrefix);
  }
  return "";
}

void CSharpMethodHelper::SetMethodProperties(const ClassMethod &method, CSharpMethod &csMethod) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    csMethod.mStaticness = method.IsStatic(mClass) ? Staticness::Static : Staticness::NotStatic;
    break;
  case CSharpMethodType::InterfaceClassAbstractMethod:
    csMethod.mStaticness = method.IsStatic(mClass) ? Staticness::Static : Staticness::NotStatic;
    csMethod.mVirtuality = Virtuality::PureVirtual;
    break;
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
    csMethod.mVirtuality = Virtuality::PureVirtual;
    csMethod.mStaticness = Staticness::Static;
    break;
  case CSharpMethodType::InterfaceClassMethodCaller:
    csMethod.mStaticness = Staticness::Static;
    break;
  }
}

CSharpType CSharpMethodHelper::ConvertReturnType(const Type &type) {
  if (type.mName == "char" && type.mType == PassByType::Pointer &&
      type.mConstness == Constness::Const) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      THROW("{} is not a valid type for this method type!", type.ToString(true));
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
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
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      return CSharpType{cls->mName};
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
      if (cls->IsProxyable()) {
        return CSharpType{"IntPtr"};
      } else if (cls->mEnum) {
        return CSharpType{cls->mName};
      } else {
        return CSharpType{std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName)};
      }
    }
  }
  if (type.mName == "std::span" || type.mName == "std::array" || type.mName == "std::vector") {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      {
        auto out = ConvertReturnType(type.mTemplateParameters.front());
        ++out.mArrayDepth;
        return out;
      }
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
      return CSharpType{"IntPtr"};
    }
  }
  THROW("Unhandled type: {}", type.ToString(true, false));
}

CSharpType CSharpMethodHelper::ConvertArgumentType(const Type &type) {
  if (type.mName == "char" && type.mType == PassByType::Pointer &&
      type.mConstness == Constness::Const) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      THROW("{} is not a valid type for this method type!", type.ToString(true));
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
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
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
      break;
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      THROW("{} is not a valid type for this method type!", type.ToString(true));
    }
    return CSharpType{"IntPtr"};
  }

  auto it = CSharpHelper::Get().CppTypeToCSharpType.find(type.mName);
  if (it != CSharpHelper::Get().CppTypeToCSharpType.end()) {
    auto out = CSharpType{it->second};
    if (type.mType == PassByType::Pointer)
      out.mType = CSharpPassByType::Out;
    return out;
  }
  if (auto cls = mProject.GetClass(type.mName)) {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
      return CSharpType{cls->mName};
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
    case CSharpMethodType::ModuleInterfaceAbstractMethod:
      if (cls->IsProxyable()) {
        return CSharpType{"IntPtr"};
      } else if (cls->mEnum) {
        return CSharpType{cls->mName};
      } else {
        return CSharpType{std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName)};
      }
    }
  }
  if (type.mName == "std::span" || type.mName == "std::array" || type.mName == "std::vector") {
    switch (mMethodType) {
    case CSharpMethodType::WrappedClassCallerMethod:
    case CSharpMethodType::WrappedClassCallerConstructor:
    case CSharpMethodType::InterfaceClassAbstractMethod:
    case CSharpMethodType::WrappedClassDelegate:
    case CSharpMethodType::ModuleInterfaceDelegate:
    case CSharpMethodType::InterfaceClassMethodCaller:
    case CSharpMethodType::InterfaceClassMethodDelegate:
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
  case CSharpMethodType::InterfaceClassMethodDelegate:
    return std::format("{}{}{}", mClass.mName, rawName, St::CSharpDelegateSuffix);
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
  case CSharpMethodType::InterfaceClassAbstractMethod:
    return rawName;
  case CSharpMethodType::InterfaceClassMethodCaller:
    return std::format("{}{}", rawName, St::CSharpInterfaceFunctionCallerSuffix);
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldAddThisArgument(const ClassMethod &method) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
  case CSharpMethodType::InterfaceClassAbstractMethod:
    return false;
  case CSharpMethodType::InterfaceClassMethodCaller:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::InterfaceClassMethodDelegate:
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
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
  case CSharpMethodType::InterfaceClassMethodCaller:
    break;
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
  case CSharpMethodType::WrappedClassCallerMethod:
    return;
  }
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
  if (!cls || cls->IsProxyable() || cls->mEnum)
    return false;
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
  case CSharpMethodType::InterfaceClassMethodCaller:
    return true;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
  case CSharpMethodType::InterfaceClassAbstractMethod:
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
  case CSharpMethodType::InterfaceClassAbstractMethod:
    return;
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::InterfaceClassMethodCaller:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  }
  if (ShouldHaveSizeArgument(type)) {
    auto &arg = arguments.emplace_back(
        std::format("{}{}", argPrefix, St::CSharpAuxiliarySizeSuffix), CSharpType{"ulong"});
    if (isReturnValue) {
      arg.mType.mType = CSharpPassByType::Out;
    }
  }
  if (isReturnValue && ShouldHaveDeleterArgument(type)) {
    auto &deleterArg =
        arguments.emplace_back(St::DeferredDeleterArgumentName, CSharpType{"IntPtr"});
    deleterArg.mType.mType = CSharpPassByType::Out;
  }
}

bool CSharpMethodHelper::ShouldHaveSizeArgument(const Type &returnType) {
  switch (mMethodType) {
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return false;
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodCaller:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    return CSharpHelper::Get().NeedsSizeArgument(returnType);
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldPassSizeArgument(const Type &returnType) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return CSharpHelper::Get().NeedsSizeArgument(returnType);
  case CSharpMethodType::InterfaceClassMethodCaller:
    return false;
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldHaveDeleterArgument(const Type &returnType) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodCaller:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    return false;
  case CSharpMethodType::WrappedClassDelegate:
    return CSharpHelper::Get().NeedsDeleterArgument(returnType);
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldPassDeleterArgument(const Type &returnType) {

  switch (mMethodType) {
  case CSharpMethodType::InterfaceClassMethodCaller:
    return false;
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return CSharpHelper::Get().NeedsDeleterArgument(returnType);
  }
  THROW("Unexpected method type!");
}

bool CSharpMethodHelper::ShouldPassThisArgument(const ClassMethod &method) {
  switch (mMethodType) {
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return !method.IsStatic(mClass) && mCsClass.mStaticness != Staticness::Static;
  case CSharpMethodType::InterfaceClassMethodCaller:
    return false;
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  }
  THROW("Unexpected method type!");
}

std::string CSharpMethodHelper::ConstructMethodArguments(const ClassMethod &method,
                                                         const CSharpMethodBase &csMethod) {
  std::stringstream ss;
  bool isFirst = true;

  auto csIt = csMethod.mArguments.begin(), csEnd = csMethod.mArguments.end();
  auto it = method.mArguments.begin(), end = method.mArguments.end();

  if (ShouldPassThisArgument(method)) {
    if (!mClass.IsProxyable() && !mClass.mEnum) {
      ss << "ref ";
    }

    ss << VariableRepresentation(Type{mCsClass.mName}, "this");
    isFirst = false;
  } else if (ShouldAddThisArgument(method)) {
    THROW_IF(csIt == csMethod.mArguments.end() && csIt->mName == St::CSharpHolgenObjectArg,
             "Unexpected method structure")
    ++csIt;
  }

  for (; it != end; ++it, ++csIt) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    THROW_IF(csIt == csEnd || it->mName != csIt->mName, "Argument order got messed up!");
    if (auto argClass = mProject.GetClass(it->mType.mName)) {
      if (!argClass->IsProxyable() && !argClass->mEnum &&
          (mMethodType == CSharpMethodType::WrappedClassCallerConstructor ||
           mMethodType == CSharpMethodType::WrappedClassCallerMethod)) {
        ss << "ref ";
      }
    }
    ss << VariableRepresentation(it->mType, csIt->mName);
    if (ShouldPassSizeArgument(it->mType)) {
      ss << std::format(", (ulong){}.Length", csIt->mName);
    } else if (ShouldHaveSizeArgument(it->mType)) {
      ++csIt;
    }
  }
  if (ShouldPassSizeArgument(method.mReturnType)) {
    if (!isFirst)
      ss << ", ";
    if (mMethodType == CSharpMethodType::WrappedClassCallerMethod)
      ss << "out var ";
    else
      ss << "out ";
    ss << St::CSharpAuxiliaryReturnValueArgName << St::CSharpAuxiliarySizeSuffix;
    isFirst = false;
  } else if (ShouldHaveSizeArgument(method.mReturnType)) {
    THROW_IF(csIt == csEnd, "Unexpected method structure");
    ++csIt;
  }

  if (ShouldPassDeleterArgument(method.mReturnType)) {
    if (!isFirst) {
      ss << ", ";
    }
    ss << "out var " << St::DeferredDeleterArgumentName;
  } else if (ShouldHaveDeleterArgument(method.mReturnType)) {
    THROW_IF(csIt == csEnd, "Unexpected method structure");
    ++csIt;
  }
  THROW_IF(csIt != csEnd, "Unexpected method structure");
  return ss.str();
}

std::string CSharpMethodHelper::VariableRepresentation(const Type &type,
                                                       const std::string &variableName) {
  auto cls = mProject.GetClass(type.mName);
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  case CSharpMethodType::InterfaceClassMethodCaller:
    if (auto cls = mProject.GetClass(type.mName)) {
      if (cls->mStruct && cls->mStruct->GetAnnotation(Annotations::DotNetInterface))
        return std::format("(({})GCHandle.FromIntPtr({}).Target!)", cls->mName, variableName);
      else if (cls->mEnum)
        return std::format("{}({})", type.mName, variableName);
      else if (cls->IsProxyable())
        return std::format("new {}({})", type.mName, variableName);
      else
        return std::format("new {}{{{} = {}}}", type.mName, St::CSharpMirroredStructFieldName,
                           variableName);
    }
    return std::format("{}", variableName);
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    if (cls && cls->mEnum)
      return std::format("{}({})", type.mName, variableName);
    else if (cls && cls->IsProxyable()) {
      return std::format("{}.{}", variableName, St::CSharpProxyObjectPointerFieldName);
    } else if (cls) {
      return std::format("{}.{}", variableName, St::CSharpMirroredStructFieldName);
    } else {
      return std::format("{}", variableName);
    }
  }
  THROW("Unexpected VariableRepresentation operation for the given method type!");
}

std::string CSharpMethodHelper::GetWrapperTargetInWrappedClass(const ClassMethod &method) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    break;
  case CSharpMethodType::InterfaceClassMethodCaller:
    {
      std::string callSuffix;
      if (!method.IsStatic(mClass)) {
        callSuffix = VariableRepresentation(Type{mCsClass.mName}, St::CSharpHolgenObjectArg) + ".";
      }
      return callSuffix + method.mName;
    }
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::WrappedClassCallerConstructor:
    return std::format("Marshal.GetDelegateForFunctionPointer<{}>({})",
                       mNaming.CSharpMethodDelegateName(mClass.mName, method.mName),
                       mNaming.CSharpMethodPointerName(method.mName));
  }
  THROW("Unexpected GetWrapperTargetInWrappedClass operation for the given method type!");
}

void CSharpMethodHelper::GenerateMethodBody(const ClassMethod &method, CSharpMethodBase &csMethod) {
  switch (mMethodType) {
  case CSharpMethodType::ModuleInterfaceDelegate:
  case CSharpMethodType::WrappedClassDelegate:
  case CSharpMethodType::ModuleInterfaceAbstractMethod:
  case CSharpMethodType::InterfaceClassAbstractMethod:
  case CSharpMethodType::InterfaceClassMethodDelegate:
    return;
  case CSharpMethodType::WrappedClassCallerMethod:
  case CSharpMethodType::InterfaceClassMethodCaller:
    return GenerateMethodBodyForMethod(method, csMethod);
  case CSharpMethodType::WrappedClassCallerConstructor:
    return GenerateMethodBodyForConstructor(method, csMethod);
  }
}

void CSharpMethodHelper::GenerateMethodBodyForConstructor(const ClassMethod &method,
                                                          CSharpMethodBase &csMethod) {
  auto caller = ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod);
  if (mClass.IsProxyable()) {
    csMethod.mBody.Add("{} = {};", St::CSharpProxyObjectPointerFieldName, caller);
  } else {
    csMethod.mBody.Add("{} = {};", St::CSharpMirroredStructFieldName, caller);
  }
}

void CSharpMethodHelper::GenerateMethodBodyForMethod(const ClassMethod &method,
                                                     CSharpMethodBase &csMethod) {
  if (method.mReturnType.mName == "void") {
    GenerateMethodBodyForMethodReturningVoid(method, csMethod);
  } else if (auto retClass = mProject.GetClass(method.mReturnType.mName)) {
    GenerateMethodBodyForMethodReturningClass(method, csMethod, *retClass);
  } else if (CSharpHelper::Get().CppTypesConvertibleToCSharpArray.contains(
                 method.mReturnType.mName)) {
    GenerateMethodBodyForMethodReturningArray(method, csMethod);
  } else {
    GenerateMethodBodyForMethodReturningValue(method, csMethod);
  }
}

void CSharpMethodHelper::GenerateMethodBodyForMethodReturningVoid(const ClassMethod &method,
                                                                  CSharpMethodBase &csMethod) {
  csMethod.mBody.Add(
      "{};", ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod));
}

void CSharpMethodHelper::GenerateMethodBodyForMethodReturningClass(const ClassMethod &method,
                                                                   CSharpMethodBase &csMethod,
                                                                   const Class &cls) {
  auto caller = ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod);
  if (mMethodType == CSharpMethodType::WrappedClassCallerMethod) {
    if (cls.mEnum) {
      csMethod.mBody.Add("return {}({});", cls.mName, caller);
    } else if (cls.IsProxyable()) {
      csMethod.mBody.Add("return new {}({});", cls.mName, caller);
    } else {
      csMethod.mBody.Add("return new {}", cls.mName);
      csMethod.mBody.Add("{{");
      csMethod.mBody.Indent(1);
      csMethod.mBody.Add("{} = {}", St::CSharpMirroredStructFieldName, caller);
      csMethod.mBody.Indent(-1);
      csMethod.mBody.Add("}};");
    }
  } else if (mMethodType == CSharpMethodType::InterfaceClassMethodCaller) {
    if (cls.mEnum) {
      csMethod.mBody.Add("return {}({});", cls.mName, caller);
    } else if (cls.IsProxyable()) {
      csMethod.mBody.Add("return {}.{};", caller, St::CSharpProxyObjectPointerFieldName);
    } else {
      csMethod.mBody.Add("return {}.{};", caller, St::CSharpMirroredStructFieldName);
    }
  } else {
    THROW("Unexpected method type!")
  }
}

void CSharpMethodHelper::GenerateMethodBodyForMethodReturningArray(const ClassMethod &method,
                                                                   CSharpMethodBase &csMethod) {
  if (mMethodType == CSharpMethodType::InterfaceClassMethodCaller) {
    GenerateMethodBodyForInterfaceClassMethodCallerReturningArray(method, csMethod);
    return;
  }

  auto caller = ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod);
  auto sizeParameter =
      std::format("{}{}", St::CSharpAuxiliaryReturnValueArgName, St::CSharpAuxiliarySizeSuffix);
  auto retVal = method.mReturnType.mTemplateParameters.front();
  auto csRetVal = ConvertReturnType(retVal);


  std::string sizeString;
  if (method.mReturnType.mName == "std::array")
    sizeString = method.mReturnType.mTemplateParameters.back().mName;
  else
    sizeString = std::format("{}Int", sizeParameter);

  std::string underlyingType = csRetVal.mName;
  auto retClass = mProject.GetClass(method.mReturnType.mTemplateParameters.front().mName);
  std::string objectConstructor;
  if (retClass) {
    if (retClass->mEnum) {
      objectConstructor = std::format("holgenResultSpan[i];", retClass->mName, caller);
    } else if (retClass->IsProxyable()) {
      underlyingType = "IntPtr";
      objectConstructor = std::format("new {}(holgenResultSpan[i])", retClass->mName);
    } else {
      underlyingType = std::format("{}.{}", retClass->mName, St::CSharpMirroredStructStructName);
      objectConstructor = std::format("new {}{{ {} = holgenResultSpan[i] }}", retClass->mName,
                                      St::CSharpMirroredStructFieldName);
    }
  } else {
    // Marshal.Copy would work better for these
    objectConstructor = "holgenResultSpan[i]";
  }
  csMethod.mBody.Add("var holgenResult = {};", caller);
  if (ShouldPassSizeArgument(method.mReturnType)) {
    csMethod.mBody.Add("var {0}Int = (int){0};", sizeParameter);
  }
  csMethod.mBody.Add("var holgenReturnValue = new {}[{}];", csRetVal.ToString(), sizeString);

  if (CSharpHelper::Get().CSharpTypesSupportedByMarshalCopy.contains(csRetVal.mName)) {
    csMethod.mBody.Add("Marshal.Copy(holgenResult, holgenReturnValue, 0, {});", sizeString);
  } else {
    csMethod.mBody.Add("Span<{}> holgenResultSpan;", underlyingType);
    csMethod.mBody.Add("unsafe");
    csMethod.mBody.Add("{{");
    csMethod.mBody.Indent(1);
    csMethod.mBody.Add("holgenResultSpan = new Span<{}>(holgenResult.ToPointer(), {});",
                       underlyingType, sizeString);
    csMethod.mBody.Indent(-1);
    csMethod.mBody.Add("}}");
    csMethod.mBody.Add("for (var i = 0; i < {}; ++i)", sizeString);
    csMethod.mBody.Add("{{");
    csMethod.mBody.Indent(1);
    csMethod.mBody.Add("holgenReturnValue[i] = {};", objectConstructor);
    csMethod.mBody.Indent(-1);
    csMethod.mBody.Add("}}");
  }
  csMethod.mBody.Add("DeferredDeleter.Perform({});", St::DeferredDeleterArgumentName);
  csMethod.mBody.Add("return holgenReturnValue;");
}

void CSharpMethodHelper::GenerateMethodBodyForInterfaceClassMethodCallerReturningArray(
    const ClassMethod &method, CSharpMethodBase &csMethod) {
  auto caller = ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod);
  auto sizeParameter =
      std::format("{}{}", St::CSharpAuxiliaryReturnValueArgName, St::CSharpAuxiliarySizeSuffix);
  auto retVal = method.mReturnType.mTemplateParameters.front();
  auto csRetVal = ConvertReturnType(retVal);

  bool isStringContainer = retVal.mName == "std::string";
  csMethod.mBody.Add("var holgenResult = {};", caller);
  if (method.mReturnType.mName != "std::array")
    csMethod.mBody.Add("{} = (ulong)holgenResult.Length;", sizeParameter);
  csMethod.mBody.Add("IntPtr holgenReturnValue;");
  csMethod.mBody.Add("unsafe");
  csMethod.mBody.Add("{{");
  csMethod.mBody.Indent(1);
  csMethod.mBody.Add("holgenReturnValue = Marshal.AllocHGlobal({} * (int){});",
                     isStringContainer || csRetVal.mName == "IntPtr"
                         ? "IntPtr.Size"
                         : std::format("sizeof({})", csRetVal.ToString()),
                     sizeParameter);
  csMethod.mBody.Indent(-1);
  csMethod.mBody.Add("}}");
  if (CSharpHelper::Get().CSharpTypesSupportedByMarshalCopy.contains(csRetVal.mName)) {
    csMethod.mBody.Add("Marshal.Copy(holgenResult, 0, holgenReturnValue, (int){});", sizeParameter);
  } else {
    csMethod.mBody.Add("for (int holgenIterator = 0; holgenIterator < (int){}; ++holgenIterator)",
                       sizeParameter);
    csMethod.mBody.Add("{{");
    csMethod.mBody.Indent(1);
    if (isStringContainer) {
      csMethod.mBody.Add(
          "IntPtr elemPtr = Marshal.StringToHGlobalAnsi(holgenResult[holgenIterator]);");
      csMethod.mBody.Add(
          "Marshal.WriteIntPtr(holgenReturnValue, holgenIterator * IntPtr.Size, elemPtr);");
    } else if (auto underlyingClass = mProject.GetClass(retVal.mName)) {
      if (underlyingClass->mEnum) {
        auto entryEnum = underlyingClass->GetNestedEnum("Entry");
        auto underlyingCsType =
            ConvertReturnType(Type{entryEnum->GetUnderlyingType(*underlyingClass)}).mName;
        std::string writer;
        std::string caster;
        if (underlyingCsType == "byte" || underlyingCsType == "sbyte" ||
            underlyingCsType == "char") {
          writer = "WriteByte";
          caster = "byte";
        } else if (underlyingCsType == "short" || underlyingCsType == "ushort") {
          writer = "WriteInt16";
          caster = "short";
        } else if (underlyingCsType == "int" || underlyingCsType == "uint") {
          writer = "WriteInt32";
          caster = "int";
        } else if (underlyingCsType == "long" || underlyingCsType == "ulong") {
          writer = "WriteInt64";
          caster = "long";
        } else {
          THROW("Could not determine how to marshal enum {}", underlyingClass->mName);
        }
        csMethod.mBody.Add("Marshal.{0}(holgenReturnValue, holgenIterator * sizeof({1}), "
                           "({1})holgenResult[holgenIterator]);",
                           writer, caster);
      } else if (underlyingClass->IsProxyable()) {
        csMethod.mBody.Add("Marshal.WriteIntPtr(holgenReturnValue, holgenIterator * IntPtr.Size, "
                           "holgenResult[holgenIterator].{});",
                           St::CSharpProxyObjectPointerFieldName);
      } else {
        csMethod.mBody.Add(
            "IntPtr destPtr = IntPtr.Add(holgenReturnValue, holgenIterator * IntPtr.Size);");
        csMethod.mBody.Add(
            "Marshal.StructureToPtr(holgenResult[holgenIterator].{}, destPtr, false);",
            St::CSharpMirroredStructFieldName);
      }
    } else if (csRetVal.mName == "ulong") {
      csMethod.mBody.Add("Marshal.WriteInt64(holgenReturnValue, holgenIterator * sizeof({}), "
                         "(long)holgenResult[holgenIterator]);",
                         csRetVal.mName);
    } else if (csRetVal.mName == "uint") {
      csMethod.mBody.Add("Marshal.WriteInt32(holgenReturnValue, holgenIterator * sizeof({}), "
                         "(long)holgenResult[holgenIterator]);",
                         csRetVal.mName);
    } else {
      THROW("Cannot marshal {}({})", csRetVal.ToString(), method.mReturnType.ToString(false));
    }
    csMethod.mBody.Indent(-1);
    csMethod.mBody.Add("}}");
  }
  csMethod.mBody.Add("return holgenReturnValue;", caller);
}

void CSharpMethodHelper::GenerateMethodBodyForMethodReturningValue(const ClassMethod &method,
                                                                   CSharpMethodBase &csMethod) {
  csMethod.mBody.Add(
      "return {};", ConstructWrapperCall(GetWrapperTargetInWrappedClass(method), method, csMethod));
}

} // namespace holgen
