#include "DotNetInterfaceClassPlugin.h"
#include "core/St.h"
#include "generator/utils/BridgingHelper.h"
#include "generator/utils/CSharpHelper.h"
#include "generator/utils/CSharpMethodHelper.h"

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
  auto caller = GenerateFunctionPointerCall(cls, method, preWork);
  method.mBody.Add(std::move(preWork));
  GenerateReturnStatement(method, caller);
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
                                                                    CodeBlock &preWork) {
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

  if (CSharpHelper::Get().NeedsSizeArgument(method.mReturnType)) {
    preWork.Add("size_t {}{} = 0;", St::CSharpAuxiliaryReturnValueArgName,
                St::CSharpAuxiliarySizeSuffix);
    if (isFirst)
      isFirst = false;
    else
      ss << ", ";
    ss << "&" << St::CSharpAuxiliaryReturnValueArgName << St::CSharpAuxiliarySizeSuffix;
  }

  ss << ")";
  return ss.str();
}

void DotNetInterfaceClassPlugin::GenerateReturnStatement(ClassMethod &method,
                                                         const std::string &valueToReturn) {
  if (method.mReturnType.mName == "void") {
    method.mBody.Add("{};", valueToReturn);
    return;
  }
  bool isBasicReturnableStatement =
      TypeInfo::Get().CppPrimitives.contains(method.mReturnType.mName) ||
      method.mReturnType.mName == "std::string" || mProject.GetClass(method.mReturnType.mName);
  if (isBasicReturnableStatement) {
    method.mBody.Add("return {};",
                     ConvertBasicStatementForReturn(valueToReturn, method.mReturnType));
    return;
  }
  method.mBody.Add("auto holgenTempValue = {};", valueToReturn);

  method.mBody.Add("{} holgenFinalValue;", method.mReturnType.ToString(true, false));
  std::string sizeParameter;
  if (method.mReturnType.mName == "std::array") {
    sizeParameter = method.mReturnType.mTemplateParameters.back().mName;
  } else {
    method.mBody.Add("holgenFinalValue.reserve({}{});", St::CSharpAuxiliaryReturnValueArgName,
                     St::CSharpAuxiliarySizeSuffix);
    sizeParameter =
        std::format("{}{}", St::CSharpAuxiliaryReturnValueArgName, St::CSharpAuxiliarySizeSuffix);
  }
  method.mBody.Add("for (size_t holgenIterator = 0; holgenIterator < {}; ++holgenIterator) {{",
                   sizeParameter);
  method.mBody.Indent(1);
  auto &underlyingType = method.mReturnType.mTemplateParameters.front();
  auto converted =
      ConvertArrayElemStatement("holgenTempValue", "holgenIterator", underlyingType);
  if (method.mReturnType.mName == "std::array")
    method.mBody.Add("holgenFinalValue[holgenIterator] = {};", converted);
  else
    method.mBody.Add("holgenFinalValue.emplace_back({});", converted);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  if (CSharpHelper::Get().CppTypesConvertibleToCSharpArray.contains(method.mReturnType.mName)) {
    if (method.mReturnType.mTemplateParameters.front().mName == "std::string")
      method.mBody.Add("{}::{}(holgenTempValue, {});", St::DeferredDeleter,
                       St::DeferredDeleterPerformManagedArray, sizeParameter);
    else
      method.mBody.Add("{}::{}(holgenTempValue);", St::DeferredDeleter,
                       St::DeferredDeleterPerformManaged);
  }
  method.mBody.Add("return holgenFinalValue;", converted);
}

std::string DotNetInterfaceClassPlugin::ConvertBasicStatementForReturn(const std::string &statement,
                                                                       const Type &type) {

  THROW_IF(type.mName == "void", "voids cant be returned");
  if (auto retClass = mProject.GetClass(type.mName)) {
    if (retClass->mStruct && retClass->mStruct->GetAnnotation(Annotations::Interface)) {
      return std::format("{}({})", retClass->mName, statement);
    } else if (retClass->IsProxyable()) {
      return std::format("*{}", statement);
    } else {
      return statement;
    }
  } else if (TypeInfo::Get().CppPrimitives.contains(type.mName) || type.mName == "std::string") {
    return statement;
  }
  THROW("Cannot convert {} for return!", type.ToString(false, false));
}

std::string DotNetInterfaceClassPlugin::ConvertArrayElemStatement(const std::string &statement,
                                                                   const std::string &iterator,
                                                                   const Type &type) {
  THROW_IF(type.mName == "void", "voids cant be returned");
  if (auto retClass = mProject.GetClass(type.mName)) {
    if (retClass->mStruct && retClass->mStruct->GetAnnotation(Annotations::Interface)) {
      return std::format("static_cast<void **>({})[{}]", statement, iterator);
    } else if (retClass->IsProxyable()) {
      return std::format("{}[{}]", statement, iterator);
    } else {
      return std::format("{}[{}]", statement, iterator);
    }
  } else if (TypeInfo::Get().CppPrimitives.contains(type.mName) || type.mName == "std::string") {
    return std::format("{}[{}]", statement, iterator);
  }
  THROW("Cannot convert {} for return!", type.ToString(false, false));
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
    csCls.mMethods.push_back(CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                                CSharpMethodType::InterfaceClassAbstractMethod)
                                 .GenerateMethod(method));
    csCls.mDelegates.push_back(CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                                  CSharpMethodType::InterfaceClassMethodDelegate)
                                   .GenerateMethod(method));
    csCls.mMethods.push_back(CSharpMethodHelper(mProject, cls, csCls, Naming(),
                                                CSharpMethodType::InterfaceClassMethodCaller)
                                 .GenerateMethod(method));
  }
}
} // namespace holgen
