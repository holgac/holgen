#include "LuaFunctionPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

#include "core/Exception.h"

namespace holgen {
void LuaFunctionPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua))
      continue;
    bool isFuncTable = false;
    if (cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
      isFuncTable = true;
      if (!IsLuaPublisher(cls)) {
        auto field =
            ClassField{Naming().FieldNameInCpp(St::LuaTable_TableField), Type{"std::string"}};
        Validate().NewField(cls, field);
        cls.mFields.push_back(std::move(field));
        GenerateTableSetter(cls);
        GenerateTableGetter(cls);
      }
    }
    ProcessStructDefinition(cls, *cls.mStruct, isFuncTable);
    if (IsLuaPublisher(cls))
      ProcessLuaPublisher(cls);
  }
}

bool LuaFunctionPlugin::IsLuaPublisher(const Class &cls) const {
  return cls.mStruct &&
      cls.mStruct->GetMatchingAnnotation(Annotations::LuaFuncTable,
                                         Annotations::LuaFuncTable_Publisher);
}

void LuaFunctionPlugin::ProcessLuaPublisher(Class &cls) const {
  GeneratePublisherRegisterSubscriber(cls);
  GeneratePublisherUnregisterSubscriber(cls);
  GeneratePublisherUnregisterSubscriberByName(cls);
  GeneratePublisherClearSubscribers(cls);
}

void LuaFunctionPlugin::GeneratePublisherRegisterSubscriber(Class &cls) const {
  FunctionDefinition func;
  func.mName = St::LuaPublisher_RegisterSubscriber;
  func.mReturnType.mType.mName = "void";
  {
    auto &arg = func.mArguments.emplace_back();
    arg.mName = "subscriber";
    arg.mType.mName = "luadata";
  }
  ProcessLuaFunction(cls, func, true);
  cls.GetMethod(St::LuaPublisher_RegisterSubscriber, Constness::NotConst)->mFunction = nullptr;
}

void LuaFunctionPlugin::GeneratePublisherUnregisterSubscriber(Class &cls) const {
  FunctionDefinition func;
  func.mName = St::LuaPublisher_UnregisterSubscriber;
  func.mReturnType.mType.mName = "void";
  {
    auto &arg = func.mArguments.emplace_back();
    arg.mName = "subscriber";
    arg.mType.mName = "luadata";
  }
  ProcessLuaFunction(cls, func, true);
  cls.GetMethod(St::LuaPublisher_UnregisterSubscriber, Constness::NotConst)->mFunction = nullptr;
}

void LuaFunctionPlugin::GeneratePublisherUnregisterSubscriberByName(Class &cls) const {
  FunctionDefinition func;
  func.mName = St::LuaPublisher_UnregisterSubscriberByName;
  func.mReturnType.mType.mName = "void";
  {
    auto &arg = func.mArguments.emplace_back();
    arg.mName = "subscriberName";
    arg.mType.mName = "string";
  }
  ProcessLuaFunction(cls, func, true);
  cls.GetMethod(St::LuaPublisher_UnregisterSubscriberByName, Constness::NotConst)->mFunction =
      nullptr;
}

void LuaFunctionPlugin::GeneratePublisherClearSubscribers(Class &cls) const {
  FunctionDefinition func;
  func.mName = St::LuaPublisher_ClearSubscribers;
  func.mReturnType.mType.mName = "void";
  ProcessLuaFunction(cls, func, true);
  cls.GetMethod(St::LuaPublisher_ClearSubscribers, Constness::NotConst)->mFunction = nullptr;
}

void LuaFunctionPlugin::GenerateTableSetter(Class &cls) const {
  auto method = ClassMethod{Naming().FieldSetterNameInCpp(St::LuaTable_TableField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("val", Type{"std::string"});
  method.mBody.Add("{} = std::move(val);", Naming().FieldNameInCpp(St::LuaTable_TableField));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::GenerateTableGetter(Class &cls) const {
  auto method = ClassMethod{Naming().FieldGetterNameInCpp(St::LuaTable_TableField),
                            Type{"std::string"}, Visibility::Public, Constness::Const};
  method.mReturnType.PreventCopying();
  method.mBody.Add("return {};", Naming().FieldNameInCpp(St::LuaTable_TableField));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::ProcessStructDefinition(Class &cls,
                                                const StructDefinition &structDefinition,
                                                bool isFuncTable) const {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin), isFuncTable);
  }
  for (auto &func: structDefinition.mFunctions) {
    if (isFuncTable || func.GetAnnotation(Annotations::LuaFunc))
      ProcessLuaFunction(cls, func, isFuncTable);
  }
}

void LuaFunctionPlugin::ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition,
                                           bool isFuncTable) const {
  if (functionDefinition.GetMatchingAnnotation(Annotations::Func, Annotations::Func_OnDataLoad) ||
      functionDefinition.GetMatchingAnnotation(Annotations::No, Annotations::No_Cpp))
    return;
  cls.mSourceIncludes.AddLibHeader("lua.hpp");
  cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
  cls.mHeaderIncludes.AddForwardDeclaration({"", "struct", "lua_State"});
  const std::string *sourceTable = nullptr;
  bool isFunctionStatic =
      functionDefinition.GetMatchingAttribute(Annotations::LuaFunc, Annotations::LuaFunc_Static);
  if (isFuncTable) {
    auto attrib = cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)
                      ->GetAttribute(Annotations::LuaFuncTable_SourceTable);
    if (attrib) {
      sourceTable = &attrib->mValue.mName;
    }

    bool isStatic = isFunctionStatic ||
        cls.mStruct->GetMatchingAttribute(Annotations::LuaFuncTable,
                                          Annotations::LuaFuncTable_Static);

    GenerateFunction(cls, functionDefinition, sourceTable, functionDefinition.mName, isFuncTable,
                     isStatic);
  } else {
    auto field =
        ClassField{Naming().LuaFunctionHandleNameInCpp(functionDefinition), Type{"std::string"}};
    auto attrib = functionDefinition.GetAnnotation(Annotations::LuaFunc)
                      ->GetAttribute(Annotations::LuaFunc_SourceTable);
    if (attrib) {
      sourceTable = &attrib->mValue.mName;
    }
    GenerateFunction(cls, functionDefinition, sourceTable, field.mName, isFuncTable,
                     isFunctionStatic);
    GenerateFunctionSetter(cls, functionDefinition, field);
    GenerateFunctionChecker(cls, functionDefinition, field);
    Validate().NewField(cls, field);
    cls.mFields.push_back(std::move(field));
  }
}

void LuaFunctionPlugin::GenerateFunctionPushArgs(
    ClassMethod &method, const FunctionDefinition &functionDefinition) const {
  for (auto &funcArg: functionDefinition.mArguments) {
    if (funcArg.mType.mName == St::Lua_CustomData) {
      method.mBody.Add("{}(luaState);", funcArg.mName);
    } else {
      // There could be a function modifier specifying whether to push mirror or proxy to lua
      method.mBody.Add("{}::{}<false>({}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                       funcArg.mName);
    }
  }
}

void LuaFunctionPlugin::GenerateFunctionChecker(Class &cls,
                                                const FunctionDefinition &functionDefinition,
                                                ClassField &functionHandle) const {
  auto method = ClassMethod{Naming().LuaFunctionCheckerNameInCpp(functionDefinition), Type{"bool"},
                            Visibility::Public, Constness::Const};
  method.mBody.Add("return !{}.empty();", functionHandle.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::GenerateFunctionSetter(Class &cls,
                                               const FunctionDefinition &functionDefinition,
                                               ClassField &functionHandle) const {
  auto method = ClassMethod{Naming().LuaFunctionSetterNameInCpp(functionDefinition), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("val", Type{"std::string"});
  method.mBody.Add("{} = val;", functionHandle.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::GenerateFunctionGetFunctionFromSourceTable(
    const std::string &functionName, bool throwOnFailure, const std::string *sourceTable,
    const std::string &functionHandle, bool isFuncTable, ClassMethod &method,
    const std::string &retVal) const {
  method.mBody.Add("lua_getglobal(luaState, \"{}\");", *sourceTable);
  if (isFuncTable) {
    method.mBody.Add("lua_pushstring(luaState, {}.c_str());",
                     Naming().FieldNameInCpp(St::LuaTable_TableField));
    method.mBody.Add("lua_gettable(luaState, -2);");
    method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
    method.mBody.Indent(1);
    method.mBody.Add("HOLGEN_WARN(\"Function table {}.{{}} not found when calling {}\", {});",
                     *sourceTable, functionName, Naming().FieldNameInCpp(St::LuaTable_TableField));
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return {};", retVal);
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
    method.mBody.Add("lua_pushstring(luaState, \"{}\");", functionHandle);
    method.mBody.Add("lua_gettable(luaState, -2);");
  } else {
    method.mBody.Add("lua_pushstring(luaState, {}.c_str());", functionHandle);
    method.mBody.Add("lua_gettable(luaState, -2);");
  }
  method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
  method.mBody.Indent(1);
  std::string warnThrowMacro = throwOnFailure ? "HOLGEN_FAIL" : "HOLGEN_WARN";
  if (isFuncTable) {
    method.mBody.Add("{}(\"Calling undefined {} function in {}.{{}}\", {});", warnThrowMacro,
                     functionName, *sourceTable, Naming().FieldNameInCpp(St::LuaTable_TableField));
  } else {
    method.mBody.Add("{}(\"Calling undefined {} function {}.{{}}\", {});", warnThrowMacro,
                     functionName, *sourceTable, functionHandle);
  }
  if (!throwOnFailure) {
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return {};", retVal);
  }
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void LuaFunctionPlugin::GenerateFunctionGetGlobalFunction(const Class &cls,
                                                          const std::string &functionName,
                                                          bool throwOnFailure,
                                                          const std::string &functionHandle,
                                                          bool isFuncTable, ClassMethod &method,
                                                          const std::string &retVal) const {
  std::string tableExpression = Naming().FieldNameInCpp(St::LuaTable_TableField);
  std::string tableExpressionRaw = tableExpression + ".c_str()";
  bool isPublisher = false;
  if (cls.mStruct->GetMatchingAnnotation(Annotations::LuaFuncTable,
                                         Annotations::LuaFuncTable_Publisher)) {
    isPublisher = true;
    tableExpression = std::format("\"{}\"", cls.mName);
    tableExpressionRaw = tableExpression;
  }
  if (isFuncTable) {
    method.mBody.Add("lua_getglobal(luaState, {});", tableExpressionRaw);

    method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
    method.mBody.Indent(1);
    if (isPublisher) {
      method.mBody.Add("HOLGEN_WARN(\"Function table {} not found when calling {}\");", cls.mName,
                       functionName);
    } else {
      method.mBody.Add("HOLGEN_WARN(\"Function table {{}} not found when calling {}\", {});",
                       functionName, tableExpression);
    }
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return {};", retVal);
    method.mBody.Indent(-1);
    method.mBody.Add("}}");

    method.mBody.Add("lua_pushstring(luaState, \"{}\");", functionHandle);
    method.mBody.Add("lua_gettable(luaState, -2);");
  } else {
    method.mBody.Add("lua_getglobal(luaState, {}.c_str());", functionHandle);
  }
  method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
  method.mBody.Indent(1);
  std::string warnThrowMacro = throwOnFailure ? "HOLGEN_FAIL" : "HOLGEN_WARN";
  if (isFuncTable) {
    method.mBody.Add("{}(\"Calling undefined {} function in {{}}\", {});", warnThrowMacro,
                     functionName, tableExpression);
  } else {
    method.mBody.Add("{}(\"Calling undefined {} function {{}}\", {});", warnThrowMacro,
                     functionName, functionHandle);
  }
  if (!throwOnFailure) {
    method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return {};", retVal);
  }
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void LuaFunctionPlugin::GenerateFunction(Class &cls, const FunctionDefinition &functionDefinition,
                                         const std::string *sourceTable,
                                         const std::string &functionHandle, bool isFuncTable,
                                         bool isStatic) const {
  auto method = NewFunction(cls, functionDefinition);
  bool isPublisher = cls.mStruct->GetMatchingAnnotation(Annotations::LuaFuncTable,
                                                        Annotations::LuaFuncTable_Publisher);

  if (isPublisher) {
    method.mStaticness = Staticness::Static;
    method.mConstness = Constness::NotConst;
    if (method.mReturnType.mName != "void") {
      auto newReturnType = Type{"std::map"};
      newReturnType.mTemplateParameters.emplace_back("std::string");
      newReturnType.mTemplateParameters.emplace_back(method.mReturnType);
      method.mReturnType = newReturnType;
    }
  } else {
    method.mConstness = Constness::Const;
  }

  method.mFunction = &functionDefinition;
  method.mArguments.emplace_front("luaState", Type{"lua_State", PassByType::Pointer});
  std::string retVal = "{}";
  if (functionDefinition.mReturnType.mCategory == FunctionReturnTypeCategory::Pointer) {
    retVal = "nullptr";
  } else if (method.mReturnType.mName == "void") {
    retVal = "void()";
  } else if (auto returnedCls = mProject.GetClass(method.mReturnType.mName)) {
    if (returnedCls->mEnum) {
      if (returnedCls->mEnum->mType == EnumDefinitionType::Bitmap) {
        retVal = std::format("{}(0)", returnedCls->mName);
      } else {
        retVal = std::format("{0}({0}::Invalid)", returnedCls->mName);
      }
    }
  }

  bool throwOnFailure =
      functionDefinition.mReturnType.mCategory == FunctionReturnTypeCategory::Reference;

  if (isFuncTable && !isPublisher) {
    if (throwOnFailure) {
      method.mBody.Add(R"(HOLGEN_FAIL_IF({}.empty(), "Calling unset {} function from table");)",
                       Naming().FieldNameInCpp(St::LuaTable_TableField), functionDefinition.mName);
    } else {
      method.mBody.Add(
          R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {}, "Calling unset {} function from table");)",
          Naming().FieldNameInCpp(St::LuaTable_TableField), retVal, functionDefinition.mName);
    }
  } else if (!isPublisher) {
    if (throwOnFailure) {
      method.mBody.Add(R"(HOLGEN_FAIL_IF({}.empty(), "Calling unset {} function");)",
                       functionHandle, functionDefinition.mName);
    } else {
      method.mBody.Add(R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {}, "Calling unset {} function");)",
                       functionHandle, retVal, functionDefinition.mName);
    }
  }

  if (sourceTable) {
    GenerateFunctionGetFunctionFromSourceTable(functionDefinition.mName, throwOnFailure,
                                               sourceTable, functionHandle, isFuncTable, method,
                                               retVal);
  } else {
    GenerateFunctionGetGlobalFunction(cls, functionDefinition.mName, throwOnFailure, functionHandle,
                                      isFuncTable, method, retVal);
  }

  int copyOffset = isFuncTable + !!sourceTable;
  if (copyOffset != 0) {
    method.mBody.Add("lua_copy(luaState, -1, {});", -(copyOffset + 1));
    method.mBody.Add("lua_pop(luaState, {});", copyOffset);
  }

  if (!isStatic) {
    method.mBody.Add("{}::{}<false>(*this, luaState);", St::LuaHelper, St::LuaHelper_Push);
  }
  GenerateFunctionPushArgs(method, functionDefinition);

  bool returnsVal = method.mReturnType.mName != "void";
  bool pushesVal =
      method.mFunction && method.mFunction->mReturnType.mType.mName == St::Lua_CustomData;
  method.mBody.Add("lua_call(luaState, {}, {});", functionDefinition.mArguments.size() + !isStatic,
                   (returnsVal || pushesVal) ? 1 : 0);

  if (returnsVal) {
    std::string returnValue = "result";
    if (auto returnClass = mProject.GetClass(method.mReturnType.mName)) {
      bool canBeMirror = !cls.HasVirtualMethods();
      bool canBeProxy = true;
      if (method.mReturnType.mType != PassByType::Value) {
        canBeMirror = false;
      }
      if (returnClass->mEnum) {
        canBeProxy = false;
      }
      if (canBeProxy && canBeMirror) {
        method.mBody.Add("{} resultMirror;", returnClass->mName);
        method.mBody.Add("{} *result;", returnClass->mName);
        method.mBody.Add("if (lua_getmetatable(luaState, -1)) {{");
        method.mBody.Indent(1);
        method.mBody.Add("lua_pop(luaState, 1);");
        method.mBody.Add("result = {}::{}(luaState, -1);", returnClass->mName,
                         St::Lua_ReadProxyObject);
        method.mBody.Indent(-1);
        method.mBody.Add("}} else {{");
        method.mBody.Indent(1);
        method.mBody.Add("resultMirror = {}::{}(luaState, -1);", returnClass->mName,
                         St::Lua_ReadMirrorObject);
        method.mBody.Add("result = &resultMirror;");
        method.mBody.Indent(-1);
        method.mBody.Add("}}");
        // This only works if function returns a value
        returnValue = "*result";
      } else if (canBeProxy) {
        method.mBody.Add("auto result = {}::{}(luaState, -1);", returnClass->mName,
                         St::Lua_ReadProxyObject);
        if (method.mReturnType.mType != PassByType::Pointer) {
          returnValue = "*result";
        }
      } else if (canBeMirror) {
        method.mBody.Add("auto result = {}::{}(luaState, -1);", returnClass->mName,
                         St::Lua_ReadMirrorObject);
      } else {
        THROW("Dont know how to get the return value of {} from lua!", method.mName);
      }
    } else {
      method.mBody.Add("{}result;", method.mReturnType.ToString(false));
      method.mBody.Add("{}::{}(result, luaState, -1);", St::LuaHelper, St::LuaHelper_Read);
    }
    if (method.mFunction->mReturnType.mType.mName != St::Lua_CustomData)
      method.mBody.Add("lua_pop(luaState, 1);");
    method.mBody.Add("return {};", returnValue);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
