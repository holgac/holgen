#include "DotNetInterfaceClassPlugin.h"
#include "core/St.h"
#include "generator/utils/BridgingHelper.h"
#include "generator/utils/CSharpHelper.h"

namespace holgen {

void DotNetInterfaceClassPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Process(cls);
  }
}

bool DotNetInterfaceClassPlugin::ShouldProcess(const Class &cls) {
  if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::Interface))
    return false;
  return true;
}

void DotNetInterfaceClassPlugin::Process(Class &cls) {
  ProcessForCpp(cls);
  ProcessForCSharp(cls);
}

void DotNetInterfaceClassPlugin::ProcessForCpp(Class &cls) {
  cls.mFriendClasses.emplace_back(St::DotNetHostName);
  cls.mHeaderIncludes.AddForwardDeclaration({mSettings.mNamespace, "class", St::DotNetHostName});
  cls.mBaseClasses.emplace_back(Visibility::Public, Type{St::CSharpInterfaceName});
  GenerateConstructorsForCpp(cls);
  for (auto &func: cls.mStruct->mFunctions) {
    GenerateFunctionForCpp(cls, func);
    GenerateFunctionPointerForCpp(cls, func);
  }
}

void DotNetInterfaceClassPlugin::GenerateConstructorsForCpp(Class &cls) {
  auto ptrFieldName = Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName);
  {
    auto &ctor = cls.mConstructors.emplace_back();
    ctor.mArguments.emplace_back("ptr", Type{"void", PassByType::Pointer});
    ctor.mInitializerList.emplace_back(St::CSharpInterfaceName, "ptr");
    ctor.mExplicitness = Explicitness::Explicit;
  }

  {
    auto &copyCtor = cls.mConstructors.emplace_back();
    copyCtor.mArguments.emplace_back("rhs",
                                     Type{cls.mName, PassByType::Reference, Constness::Const});
    copyCtor.mDefaultDelete = DefaultDelete::Delete;
  }
  {
    auto &copyOp = cls.mMethods.emplace_back(
        "operator=", Type{cls.mName, PassByType::Reference, Constness::Const});
    copyOp.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    copyOp.mDefaultDelete = DefaultDelete::Delete;
  }
  {
    auto &moveCtor = cls.mConstructors.emplace_back();
    moveCtor.mNoexceptness = Noexceptness::Noexcept;
    moveCtor.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    moveCtor.mInitializerList.emplace_back(St::CSharpInterfaceName, "std::move(rhs)");
  }
  {
    auto &moveOp = cls.mMethods.emplace_back("operator=", Type{cls.mName, PassByType::Reference},
                                             Visibility::Public, Constness::NotConst);
    moveOp.mNoexceptness = Noexceptness::Noexcept;
    moveOp.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::MoveReference});
    moveOp.mBody.Add("std::swap({0}, rhs.{0});", ptrFieldName);
    moveOp.mBody.Add("return *this;");
  }
}

void DotNetInterfaceClassPlugin::GenerateFunctionForCpp(
    Class &cls, const FunctionDefinition &functionDefinition) {
  auto method = GenerateFunction(cls, functionDefinition, false, false);
  method.mExposeToScript = true;

  CodeBlock preWork, postWork;
  auto caller = GenerateFunctionPointerCall(cls, method, preWork, postWork);

  method.mBody.Add(std::move(preWork));

  if (method.mReturnType.mName == "void") {
    method.mBody.Add("{};", caller);
    method.mBody.Add(std::move(postWork));
  } else {
    if (auto retClass = mProject.GetClass(method.mReturnType.mName)) {
      if (retClass->mStruct && retClass->mStruct->GetAnnotation(Annotations::Interface)) {
        caller = std::format("{}({})", retClass->mName, caller);
      } else if (retClass->IsProxyable()) {
        caller = std::format("*{}", caller);
      } else {
        caller = std::format("{}", caller);
      }
    } else {
      // TODO: handle vectors and such
    }
    if (postWork.IsEmpty())
      method.mBody.Add("return {};", caller);
    else {
      method.mBody.Add("auto holgenFinalRes = {};", caller);
      method.mBody.Add(std::move(postWork));
      method.mBody.Add("return holgenFinalRes;");
    }
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void DotNetInterfaceClassPlugin::GenerateFunctionPointerForCpp(
    Class &cls, const FunctionDefinition &functionDefinition) {
  auto method = GenerateFunction(cls, functionDefinition, true, true);
  method.mExposeToScript = false;
  method.mName += St::CSharpInterfaceFunctionSuffix;
  method.mFunctionPointer = true;
  method.mVisibility = Visibility::Private;
  method.mStaticness = Staticness::Static;
  if (auto retClass = mProject.GetClass(method.mReturnType.mName)) {
    if (retClass->mStruct && retClass->mStruct->GetAnnotation(Annotations::Interface)) {
      method.mReturnType = Type{"void", PassByType::Pointer};
    }
  }
  for (auto &arg: method.mArguments) {
    auto argClass = mProject.GetClass(arg.mType.mName);
    if (argClass && argClass->mStruct && argClass->mStruct->GetAnnotation(Annotations::Interface)) {
      arg.mType = Type{"void", PassByType::Pointer};
    }
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

ClassMethod
    DotNetInterfaceClassPlugin::GenerateFunction(Class &cls,
                                                 const FunctionDefinition &functionDefinition,
                                                 bool convertArguments, bool addThisArgument) {
  auto method = NewFunction(cls, functionDefinition);
  auto oldArgs = std::move(method.mArguments);
  method.mArguments.clear();
  if (addThisArgument && !method.IsStatic(cls)) {
    method.mArguments.emplace_back(St::CSharpInterfaceInstanceName,
                                   Type{cls.mName, PassByType::Pointer});
  }
  if (convertArguments) {
    for (auto &oldArg: oldArgs) {
      auto &newArg = BridgingHelper::AddArgument(mProject, method, oldArg,
                                                 functionDefinition.mDefinitionSource);
      if (newArg.mType.mType == PassByType::Pointer &&
          CSharpHelper::Get().CppTypesConvertibleToCSharpArray.contains(oldArg.mType.mName)) {
        newArg.mType.mConstness = Constness::Const;
      }
    }
    BridgingHelper::AddAuxiliaryArguments(mProject, method, method.mReturnType,
                                          St::CSharpAuxiliaryReturnValueArgName, true);
    method.mReturnType = BridgingHelper::ConvertType(mProject, method.mReturnType, true,
                                                     functionDefinition.mDefinitionSource);
  } else {
    method.mArguments.insert(method.mArguments.end(), oldArgs.begin(), oldArgs.end());
  }
  return method;
}

std::string DotNetInterfaceClassPlugin::GenerateFunctionPointerCall(const Class &cls,
                                                                    const ClassMethod &method,
                                                                    CodeBlock &preWork,
                                                                    CodeBlock &postWork) {
  (void)postWork;
  std::stringstream ss;
  ss << method.mName + St::CSharpInterfaceFunctionSuffix << "(";
  bool isFirst = true;
  if (!method.IsStatic(cls)) {
    ss << Naming().FieldNameInCpp(St::CSharpInterfaceInstanceName);
    isFirst = false;
  }
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    auto argClass = mProject.GetClass(arg.mType.mName);
    std::string toPointer = arg.mType.mType != PassByType::Pointer ? "&" : "";
    if (argClass) {
      if (argClass && argClass->mStruct &&
          argClass->mStruct->GetAnnotation(Annotations::Interface)) {
        ss << arg.mName << "." << Naming().FieldGetterNameInCpp(St::CSharpInterfaceInstanceName)
           << "()";
      } else {
        ss << toPointer << arg.mName;
      }
    } else if (CSharpHelper::Get().CppTypesConvertibleToCSharpArray.contains(arg.mType.mName)) {
      auto finalArgName = arg.mName;
      auto underlying = arg.mType.mTemplateParameters.front();
      auto underlyingCls = mProject.GetClass(underlying.mName);
      if (underlying.mName == "std::string" || underlyingCls) {
        bool isInterface = underlyingCls && underlyingCls->mStruct &&
            underlyingCls->mStruct->GetAnnotation(Annotations::Interface);
        finalArgName = std::format("{}HolgenTemp", arg.mName);
        std::string convertedElemName = "elem";
        if (isInterface)
          convertedElemName =
              "elem." + Naming().FieldGetterNameInCpp(St::CSharpInterfaceInstanceName);
        else if (underlying.mName == "std::string")
          convertedElemName = "elem.c_str()";


        if (isInterface)
          preWork.Add("std::vector<void*> {}HolgenTemp;", arg.mType.ToString(true, false),
                      arg.mName);
        else {
          auto typ = BridgingHelper::ConvertType(mProject, underlying, false, {});
          // typ.mConstness = Constness::NotConst;
          // typ.mType = PassByType::Value;
          preWork.Add("std::vector<{}> {}HolgenTemp;", typ.ToString(true, false), arg.mName);
        }
        preWork.Add("{0}HolgenTemp.reserve({0}.size());", arg.mName);
        preWork.Add("for (auto& elem: {}) {{", arg.mName);
        preWork.Indent(1);
        preWork.Add("{}HolgenTemp.push_back({});", arg.mName, convertedElemName);
        preWork.Indent(-1);
        preWork.Add("}}");
      }

      ss << finalArgName << ".data()";
      if (arg.mType.mName != "std::array") {
        ss << ", " << finalArgName << ".size()";
      }
    } else if (arg.mType.mName == "std::string") {
      ss << arg.mName << ".c_str()";
    } else {
      ss << arg.mName;
    }
  }

  ss << ")";
  return ss.str();
}

void DotNetInterfaceClassPlugin::ProcessForCSharp(Class &cls) {
  auto &csCls = GenerateCSharpClass(cls);
  csCls.mUsingDirectives.emplace("System.Runtime.InteropServices");
  GenerateCSharpPointerField(csCls);
  GenerateCSharpMethods(cls, csCls);
}

CSharpClass &DotNetInterfaceClassPlugin::GenerateCSharpClass(Class &cls) {
  auto csCls = CSharpClass{cls.mName, &cls};
  csCls.mIsAbstract = true;
  mProject.mCSharpClasses.push_back(std::move(csCls));
  return mProject.mCSharpClasses.back();
}

void DotNetInterfaceClassPlugin::GenerateCSharpPointerField(CSharpClass &csCls) {
  auto ptrName = Naming().PrivateFieldNameInCSharp(St::CSharpProxyObjectPointerFieldName);
  auto &ptrAccessor = csCls.mFields.emplace_back(St::CSharpProxyObjectPointerFieldName,
                                                 CSharpType{"IntPtr"}, CSharpVisibility::Public);

  ptrAccessor.mGetter = CSharpMethodBase{};
  ptrAccessor.mGetter->mBody.Add("if (!{}.IsAllocated)", ptrName);
  ptrAccessor.mGetter->mBody.Add("{{");
  ptrAccessor.mGetter->mBody.Indent(1);
  ptrAccessor.mGetter->mBody.Add("{} = GCHandle.Alloc(this);", ptrName);
  ptrAccessor.mGetter->mBody.Indent(-1);
  ptrAccessor.mGetter->mBody.Add("}}");
  ptrAccessor.mGetter->mBody.Add("return (IntPtr){};", ptrName);

  csCls.mFields.emplace_back(ptrName, CSharpType{"GCHandle"}, CSharpVisibility::Private,
                             Staticness::NotStatic);
}

void DotNetInterfaceClassPlugin::GenerateCSharpMethods(const Class &cls, CSharpClass &csCls) {
  for (auto &method: cls.mMethods) {
    // work with unprocessed methods to retain more info about user intent
    if (!method.mExposeToScript)
      continue;
    auto &csMethod = GenerateCSharpAbstractMethod(cls, csCls, method);
    GenerateCSharpMethodCallerMethod(cls, csCls, method, csMethod);
  }
}

CSharpMethod &DotNetInterfaceClassPlugin::GenerateCSharpAbstractMethod(const Class &cls,
                                                                       CSharpClass &csCls,
                                                                       const ClassMethod &method) {
  auto csMethod = CSharpHelper::Get().CreateMethod(mProject, cls, method, InteropType::Internal,
                                                   InteropType::Internal, false, true);
  csMethod.mVirtuality = Virtuality::PureVirtual;
  for (auto &arg: csMethod.mArguments) {
    arg.mAttributes.clear();
  }
  csCls.mMethods.push_back(std::move(csMethod));
  return csCls.mMethods.back();
}

void DotNetInterfaceClassPlugin::GenerateCSharpMethodCallerMethod(const Class &cls,
                                                                  CSharpClass &csCls,
                                                                  const ClassMethod &method,
                                                                  const CSharpMethod &csMethod) {
  auto callerMethod =
      CSharpHelper::Get().CreateMethod(mProject, cls, method, InteropType::NativeToManaged,
                                       InteropType::NativeToManaged, true, false);

  auto oldName = callerMethod.mName;
  callerMethod.mName = Naming().CSharpMethodDelegateName(cls.mName, method.mName);
  callerMethod.mStaticness = Staticness::Static;
  csCls.mDelegates.push_back(callerMethod);
  callerMethod.mName = oldName + St::CSharpInterfaceFunctionCallerSuffix;

  std::string callSuffix;
  if (!method.IsStatic(cls)) {
    auto repr = CSharpHelper::Get().VariableRepresentation(
        CSharpType{csCls.mName}, St::CSharpHolgenObjectArg, mProject, InteropType::NativeToManaged);
    callSuffix = std::format("{}.", repr);
  }

  CSharpHelper::Get().GenerateWrapperCall(
      callerMethod.mBody, mProject, InteropType::NativeToManaged, InteropType::ManagedToNative,
      callSuffix + method.mName, csCls, method, csMethod, false, true);

  csCls.mMethods.push_back(std::move(callerMethod));
}

std::string DotNetInterfaceClassPlugin::GenerateCSharpMethodCall(const Class &cls,
                                                                 CSharpClass &csCls,
                                                                 const ClassMethod &method,
                                                                 const CSharpMethod &csMethod) {
  (void)cls;
  (void)csCls;
  std::stringstream ss;
  ss << method.mName << "(";
  bool isFirst = true;
  auto it = method.mArguments.begin(), end = method.mArguments.end();
  auto csIt = csMethod.mArguments.begin(), csEnd = csMethod.mArguments.end();

  for (; it != end; ++it, ++csIt) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    THROW_IF(csIt == csEnd || csIt->mName != it->mName, "Argument order got messed up!")
    ss << CSharpHelper::Get().VariableRepresentation(csIt->mType, csIt->mName, mProject,
                                                     InteropType::NativeToManaged);
  }

  ss << ")";
  return ss.str();
}

} // namespace holgen
