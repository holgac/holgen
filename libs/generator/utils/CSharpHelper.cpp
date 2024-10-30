#include "CSharpHelper.h"

#include "generator/TranslatedProject.h"

#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
bool CSharpHelper::NeedsDeleter(const Type &type) const {
  return CppTypesConvertibleToCSharpArray.contains(type.mName);
}

bool CSharpHelper::NeedsSizeArgument(const Type &type) const {
  return type.mName == "std::vector" || type.mName == "std::span";
}

CSharpType CSharpHelper::ConvertType(const Type &type, const TranslatedProject &project,
                                     InteropType interopType, bool returnType) const {
  (void)interopType;
  CSharpType out;
  auto cls = project.GetClass(type.mName);
  auto it = CppTypeToCSharpType.find(type.mName);
  if (type.mName == "char" && type.mType == PassByType::Pointer) {
    out.mName = "string";
    out.mArrayDepth = type.mPointerDepth;
    THROW_IF(out.mArrayDepth > 1, "{} cannot be represented in c#: pointer level too deep!",
             type.ToString(true));
  } else if (it != CppTypeToCSharpType.end()) {
    out.mName = it->second;
  } else if (cls) {
    auto csCls = project.GetCSharpClass(type.mName);
    if (interopType == InteropType::NativeToManaged ||
        (csCls && csCls->mStaticness == Staticness::Static)) {
      if (cls->IsProxyable()) {
        out.mName = "IntPtr";
      } else {
        out.mName = std::format("{}.{}", cls->mName, St::CSharpMirroredStructStructName);
      }
    } else {
      out.mName = cls->mName;
    }
  } else if (type.mName == "std::span" || type.mName == "std::array" ||
             type.mName == "std::vector") {
    if (interopType == InteropType::NativeToManaged && returnType) {
      out.mName = "IntPtr";
    } else {
      out = ConvertType(type.mTemplateParameters.front(), project, interopType, false);
      ++out.mArrayDepth;
    }
  } else {
    THROW("Unexpected type: {}", type.mName);
  }
  return out;
}

void CSharpHelper::AddAttributes(std::list<std::string> &attributes, const Type &type,
                                 const CSharpType &csType, InteropType interopType,
                                 bool isReturnType, size_t sizeArgIndex) const {
  (void)interopType;
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

void CSharpHelper::AddAuxiliaryArguments(std::list<CSharpMethodArgument> &arguments,
                                         const Type &type, const std::string &argPrefix,
                                         InteropType interopType, bool isReturnValue) const {
  (void)interopType;
  if (type.mName == "std::vector" || type.mName == "std::span") {
    auto &arg = arguments.emplace_back(
        std::format("{}{}", argPrefix, St::CSharpAuxiliarySizeSuffix), CSharpType{"ulong"});
    if (isReturnValue) {
      arg.mType.mType = CSharpPassByType::Out;
    }
  }
  if (isReturnValue &&
      (type.mName == "std::array" || type.mName == "std::vector" || type.mName == "std::span")) {
    auto &deleterArg =
        arguments.emplace_back(St::DeferredDeleterArgumentName, CSharpType{"IntPtr"});
    deleterArg.mType.mType = CSharpPassByType::Out;
  }
}

std::string CSharpHelper::StringifyPassedExtraArguments(const Type &type,
                                                        const std::string &argPrefix,
                                                        InteropType interopType) const {
  (void)interopType;
  if (type.mName == "std::vector" || type.mName == "std::span") {
    return std::format(", (ulong){}.Length", argPrefix);
  }
  return "";
}

std::string CSharpHelper::RepresentationInNative(const Type &other, const Type &originalType,
                                                 const TranslatedProject &project,
                                                 bool prependRef) const {
  (void)originalType;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    switch (other.mPointerDepth) {
    case 0:
      return "string";
    case 1:
      return "string[]";
    default:
      THROW("Unexpected pointer-to-pointer-to-pointer in {}", other.ToString(true, false));
    }
  }

  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    if (other.mType == PassByType::Pointer)
      return std::format("{}[]", it->second);
    return it->second;
  }
  if (auto cls = project.GetClass(other.mName)) {
    if (cls->IsProxyable()) {
      return "IntPtr";
    } else {
      return std::format("{}{}.{}", prependRef ? "ref " : "", other.mName,
                         St::CSharpMirroredStructStructName);
    }
  }
  return other.mName;
}

std::string CSharpHelper::RepresentationInManaged(const Type &other, const Type &originalType,
                                                  const TranslatedProject &project) const {
  (void)originalType;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    switch (other.mPointerDepth) {
    case 0:
      return "string";
    case 1:
      return "string[]";
    default:
      THROW("Unexpected pointer-to-pointer-to-pointer in {}", other.ToString(true, false));
    }
  }

  (void)project;
  auto it = CppTypeToCSharpType.find(other.mName);
  if (it != CppTypeToCSharpType.end()) {
    if (other.mType == PassByType::Pointer)
      return std::format("{}[]", it->second);
    return it->second;
  }
  return other.mName;
}

std::string CSharpHelper::MarshallingInfo(const Type &other, const TranslatedProject &project,
                                          InteropType interopType) const {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
  case InteropType::NativeToManaged:
    return MarshallingInfo(other, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                               InteropType interopType,
                                               size_t sizeArgumentIdx) const {
  switch (interopType) {
  case InteropType::Internal:
    return other.mName;
  case InteropType::ManagedToNative:
  case InteropType::NativeToManaged:
    return ArrayMarshallingInfo(other, project, sizeArgumentIdx);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

std::string CSharpHelper::VariableRepresentation(const CSharpType &type,
                                                 const std::string &variableName,
                                                 const TranslatedProject &project,
                                                 InteropType interopType) const {
  switch (interopType) {
  case InteropType::Internal:
    return variableName;
  case InteropType::ManagedToNative:
    return VariableRepresentationInNative(type, variableName, project);
  case InteropType::NativeToManaged:
    return VariableRepresentationInManaged(type, variableName, project);
  }
  THROW("Unexpected interop type: {}", uint32_t(interopType));
}

CSharpMethod CSharpHelper::CreateMethod(const TranslatedProject &project, const Class &cls,
                                        const ClassMethod &method, InteropType argsInteropType,
                                        InteropType returnTypeInteropType, bool addThisArgument,
                                        bool ignoreAuxiliaries) const {

  auto csMethod = CSharpMethod{
      method.mName, ConvertType(method.mReturnType, project, returnTypeInteropType, true)};
  if (addThisArgument) {
    auto &arg = csMethod.mArguments.emplace_back(
        St::CSharpHolgenObjectArg, ConvertType(Type{cls.mName}, project, argsInteropType, true));
    if (!cls.IsProxyable())
      arg.mType.mType = CSharpPassByType::Ref;
  }
  PopulateArguments(project, csMethod.mArguments, method.mArguments, argsInteropType,
                    ignoreAuxiliaries);
  // AddAttributes(csMethod.mAttributes, method.mReturnType,
  // csMethod.mReturnType, argsInteropType, true, csMethod.mArguments.size());
  if (!ignoreAuxiliaries) {
    AddAuxiliaryArguments(csMethod.mArguments, method.mReturnType,
                          St::CSharpAuxiliaryReturnValueArgName, argsInteropType, true);
  }
  return csMethod;
}

CSharpConstructor CSharpHelper::CreateConstructor(const TranslatedProject &project,
                                                  const Class &cls, const ClassMethod &method,
                                                  InteropType interopType) const {
  (void)cls;
  auto csCtor = CSharpConstructor{};
  PopulateArguments(project, csCtor.mArguments, method.mArguments, interopType, true);
  return csCtor;
}

std::string CSharpHelper::VariableRepresentationInNative(const CSharpType &type,
                                                         const std::string &variableName,
                                                         const TranslatedProject &project) const {
  if (auto cls = project.GetClass(type.mName)) {
    if (cls->IsProxyable()) {
      return std::format("{}{}.{}", type.mType, variableName,
                         St::CSharpProxyObjectPointerFieldName);
    } else {
      return std::format("{}{}.{}", type.mType, variableName, St::CSharpMirroredStructFieldName);
    }
  }
  return std::format("{}{}", type.mType, variableName);
}

std::string CSharpHelper::VariableRepresentationInManaged(const CSharpType &type,
                                                          const std::string &variableName,
                                                          const TranslatedProject &project) const {

  if (auto cls = project.GetClass(type.mName)) {
    if (cls->IsProxyable())
      return std::format("new {}({})", type.mName, variableName);
    else
      return std::format("new {}{{{} = {}}}", type.mName, St::CSharpMirroredStructFieldName,
                         variableName);
  }
  return std::format("{}{}", type.mType, variableName);
}

std::string CSharpHelper::MarshallingInfo(const Type &other,
                                          const TranslatedProject &project) const {
  (void)project;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    return "[MarshalAs(UnmanagedType.LPStr)] ";
  }
  return "";
}

std::string CSharpHelper::ArrayMarshallingInfo(const Type &other, const TranslatedProject &project,
                                               size_t sizeArgumentIdx) const {
  (void)project;
  if (other.mName == "char" && other.mType == PassByType::Pointer) {
    return std::format(
        "[MarshalAs(UnmanagedType.LPArray, SizeParamIndex={}, ArraySubType=UnmanagedType.LPStr)] ",
        sizeArgumentIdx);
  }
  return std::format("[MarshalAs(UnmanagedType.LPArray, SizeParamIndex={})] ", sizeArgumentIdx);
}

void CSharpHelper::PopulateArguments(const TranslatedProject &project,
                                     std::list<CSharpMethodArgument> &out,
                                     const std::list<MethodArgument> &arguments,
                                     InteropType interopType, bool ignoreAuxiliaries) const {
  for (auto &arg: arguments) {
    auto csType = ConvertType(arg.mType, project, interopType, false);
    auto &csArg = out.emplace_back(arg.mName, csType, arg.mDefaultValue);
    AddAttributes(csArg.mAttributes, arg.mType, csType, interopType, false, out.size());
    auto argClass = project.GetClass(arg.mType.mName);
    if (argClass && !argClass->IsProxyable() && interopType != InteropType::Internal) {
      csArg.mType.mType = CSharpPassByType::Ref;
    }
    if (!ignoreAuxiliaries) {
      AddAuxiliaryArguments(out, arg.mType, csArg.mName, interopType, false);
    }
  }
}

CSharpHelper &CSharpHelper::Get() {
  static CSharpHelper instance;
  return instance;
}

CSharpHelper::CSharpHelper() {
  CppTypeToCSharpType = {
      {"int8_t", "sbyte"},       {"int16_t", "short"},       {"int32_t", "int"},
      {"int64_t", "long"},       {"std::ptrdiff_t", "long"}, {"uint8_t", "byte"},
      {"uint16_t", "ushort"},    {"uint32_t", "uint"},       {"uint64_t", "ulong"},
      {"size_t", "ulong"},       {"float", "float"},         {"double", "double"},
      {"std::string", "string"}, {"void", "void"},           {"bool", "bool"}};
  CppTypesConvertibleToCSharpArray = {"std::vector", "std::array", "std::span"};
  CSharpTypesSupportedByMarshalCopy = {"byte", "short", "int", "long", "float", "double"};
}

void CSharpHelper::GenerateWrapperCallReturningValue(
    CodeBlock &codeBlock, const TranslatedProject &project, InteropType argsInteropType,
    const std::string &methodToCall, const CSharpClass &cls, const ClassMethod &method,
    const CSharpMethodBase &csMethod, bool addThisArgument) const {
  auto caller = ConstructWrapperCall(cls, project, argsInteropType, methodToCall, method, csMethod,
                                     addThisArgument, false, false);
  codeBlock.Add("return {};", caller);
}

void CSharpHelper::GenerateWrapperCallReturningClass(
    CodeBlock &codeBlock, const TranslatedProject &project, InteropType argsInteropType,
    InteropType returnTypeInteropType, const std::string &methodToCall, const CSharpClass &cls,
    const ClassMethod &method, const CSharpMethodBase &csMethod, bool addThisArgument,
    const Class &returnType) const {
  auto caller = ConstructWrapperCall(cls, project, argsInteropType, methodToCall, method, csMethod,
                                     addThisArgument, false, false);
  if (returnTypeInteropType == InteropType::NativeToManaged) {
    if (returnType.IsProxyable()) {
      codeBlock.Add("return new {}({});", returnType.mName, caller);
    } else {
      codeBlock.Add("return new {}", returnType.mName);
      codeBlock.Add("{{");
      codeBlock.Indent(1);
      codeBlock.Add("{} = {}", St::CSharpMirroredStructFieldName, caller);
      codeBlock.Indent(-1);
      codeBlock.Add("}};");
    }
  } else {
    if (returnType.IsProxyable()) {
      codeBlock.Add("return {}.{};", caller, St::CSharpProxyObjectPointerFieldName);
    } else {
      codeBlock.Add("return {}.{};", caller, St::CSharpMirroredStructFieldName);
    }
  }
}

std::string CSharpHelper::ConstructWrapperCall(
    const CSharpClass &cls, const TranslatedProject &project, InteropType interopType,
    const std::string &methodToCall, const MethodBase &method, const CSharpMethodBase &csMethod,
    bool addThisArgument, bool hasSizeArg, bool hasDeleterArg) const {
  return std::format("{}({})", methodToCall,
                     ConstructMethodArguments(cls, project, method, csMethod, interopType,
                                              addThisArgument, hasSizeArg, hasDeleterArg));
}

void CSharpHelper::GenerateWrapperCall(CodeBlock &codeBlock, const TranslatedProject &project,
                                       InteropType argsInteropType,
                                       InteropType returnTypeInteropType,
                                       const std::string &methodToCall, const CSharpClass &cls,
                                       const ClassMethod &method, const CSharpMethodBase &csMethod,
                                       bool addThisArgument) const {
  if (method.mReturnType.mName == "void") {
    GenerateWrapperCallReturningVoid(codeBlock, project, argsInteropType, methodToCall, cls, method,
                                     csMethod, addThisArgument);
  } else if (auto retClass = project.GetClass(method.mReturnType.mName)) {
    GenerateWrapperCallReturningClass(codeBlock, project, argsInteropType, returnTypeInteropType,
                                      methodToCall, cls, method, csMethod, addThisArgument,
                                      *retClass);
  } else if (CppTypesConvertibleToCSharpArray.contains(method.mReturnType.mName)) {
    GenerateWrapperCallReturningArray(codeBlock, project, argsInteropType, returnTypeInteropType,
                                      methodToCall, cls, method, csMethod, addThisArgument);
  } else {
    GenerateWrapperCallReturningValue(codeBlock, project, argsInteropType, methodToCall, cls,
                                      method, csMethod, addThisArgument);
  }
}

void CSharpHelper::GenerateWrapperCallReturningVoid(
    CodeBlock &codeBlock, const TranslatedProject &project, InteropType interopType,
    const std::string &methodToCall, const CSharpClass &cls, const ClassMethod &method,
    const CSharpMethodBase &csMethod, bool addThisArgument) const {
  auto caller = ConstructWrapperCall(cls, project, interopType, methodToCall, method, csMethod,
                                     addThisArgument, false, false);
  codeBlock.Add("{};", caller);
}

void CSharpHelper::GenerateWrapperCallReturningArray(
    CodeBlock &codeBlock, const TranslatedProject &project, InteropType argsInteropType,
    InteropType returnTypeInteropType, const std::string &methodToCall, const CSharpClass &cls,
    const ClassMethod &method, const CSharpMethodBase &csMethod, bool addThisArgument) const {
  (void)returnTypeInteropType;
  bool hasSizeArg = NeedsSizeArgument(method.mReturnType);
  auto caller = ConstructWrapperCall(cls, project, argsInteropType, methodToCall, method, csMethod,
                                     addThisArgument, hasSizeArg, true);
  auto sizeParameter =
      std::format("{}{}", St::CSharpAuxiliaryReturnValueArgName, St::CSharpAuxiliarySizeSuffix);

  std::string sizeString;
  if (method.mReturnType.mName == "std::array")
    sizeString = method.mReturnType.mTemplateParameters.back().mName;
  else
    sizeString = std::format("{}Int", sizeParameter);

  auto retVal = method.mReturnType.mTemplateParameters.front();
  auto csRetVal = ConvertType(retVal, project, InteropType::Internal, true);
  std::string underlyingType = csRetVal.mName;
  auto retClass = project.GetClass(method.mReturnType.mTemplateParameters.front().mName);
  std::string objectConstructor;
  if (retClass) {
    if (retClass->IsProxyable()) {
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
  codeBlock.Add("var holgenResult = {};", caller);
  if (hasSizeArg) {
    codeBlock.Add("var {0}Int = (int){0};", sizeParameter);
  }
  codeBlock.Add("var holgenReturnValue = new {}[{}];", csRetVal.ToString(), sizeString);

  if (CSharpHelper::Get().CSharpTypesSupportedByMarshalCopy.contains(csRetVal.mName)) {
    codeBlock.Add("Marshal.Copy(holgenResult, holgenReturnValue, 0, {});", sizeString);
  } else {
    codeBlock.Add("Span<{}> holgenResultSpan;", underlyingType);
    codeBlock.Add("unsafe");
    codeBlock.Add("{{");
    codeBlock.Indent(1);
    codeBlock.Add("holgenResultSpan = new Span<{}>(holgenResult.ToPointer(), {});", underlyingType,
                  sizeString);
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
    codeBlock.Add("for (var i = 0; i < {}; ++i)", sizeString);
    codeBlock.Add("{{");
    codeBlock.Indent(1);
    codeBlock.Add("holgenReturnValue[i] = {};", objectConstructor);
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
  codeBlock.Add("DeferredDeleter.Perform({});", St::DeferredDeleterArgumentName);
  codeBlock.Add("return holgenReturnValue;");
}

std::string CSharpHelper::ConstructMethodArguments(const CSharpClass &cls,
                                                   const TranslatedProject &project,
                                                   const MethodBase &method,
                                                   const CSharpMethodBase &csMethod,
                                                   InteropType interopType, bool addThisArgument,
                                                   bool hasSizeArg, bool hasDeleterArg) const {
  std::stringstream ss;
  bool isFirst = true;

  if (addThisArgument) {
    if (interopType != InteropType::NativeToManaged && cls.mClass && !cls.mClass->IsProxyable()) {
      ss << "ref ";
    }

    ss << VariableRepresentation(CSharpType{cls.mName}, "this", project, interopType);
    isFirst = false;
  }


  auto csIt = csMethod.mArguments.begin(), csEnd = csMethod.mArguments.end();
  auto prevCsIt = csIt;
  auto it = method.mArguments.begin(), end = method.mArguments.end();
  for (; csIt != csEnd; ++it, ++csIt) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    THROW_IF(it == end || it->mName != csIt->mName, "Argument order got messed up!");
    if (prevCsIt->mName + St::CSharpAuxiliarySizeSuffix == csIt->mName) {
      auto variableName = std::format("(ulong){}.Length", prevCsIt->mName);
      ss << VariableRepresentation(csIt->mType, variableName, project, interopType);
      continue;
    }
    if (auto argClass = project.GetClass(it->mType.mName)) {
      if (interopType != InteropType::NativeToManaged && !argClass->IsProxyable()) {
        ss << "ref ";
      }
    }
    ss << VariableRepresentation(csIt->mType, csIt->mName, project, interopType);
    ss << StringifyPassedExtraArguments(it->mType, csIt->mName, interopType);
    prevCsIt = csIt;
  }
  if (hasSizeArg) {
    if (!isFirst)
      ss << ", ";
    ss << "out var " << St::CSharpAuxiliaryReturnValueArgName << St::CSharpAuxiliarySizeSuffix;
    isFirst = false;
  }

  if (hasDeleterArg) {
    if (!isFirst) {
      ss << ", ";
    }
    ss << "out var " << St::DeferredDeleterArgumentName;
  }
  return ss.str();
}

std::string CSharpHelper::GetWrapperTargetInWrappedClass(const CSharpClass &cls,
                                                         const NamingConvention &naming,
                                                         const std::string &methodName) const {
  return std::format("Marshal.GetDelegateForFunctionPointer<{}>({})",
                     naming.CSharpMethodDelegateName(cls.mName, methodName),
                     naming.CSharpMethodPointerName(methodName));
}

} // namespace holgen
