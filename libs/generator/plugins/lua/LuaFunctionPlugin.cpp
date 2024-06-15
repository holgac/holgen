#include "LuaFunctionPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void LuaFunctionPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr || cls.mStruct->GetAnnotation(Annotations::NoLua))
      continue;
    bool isFuncTable = false;
    if (cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
      isFuncTable = true;
      auto field =
          ClassField{Naming().FieldNameInCpp(St::LuaTable_TableField), Type{"std::string"}};
      Validate().NewField(cls, field);
      cls.mFields.push_back(std::move(field));
      GenerateTableSetter(cls);
    }
    ProcessStructDefinition(cls, *cls.mStruct, isFuncTable);
  }
}

void LuaFunctionPlugin::GenerateTableSetter(Class &cls) {
  auto method = ClassMethod{Naming().FieldSetterNameInCpp(St::LuaTable_TableField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("val", Type{"std::string"});
  method.mBody.Add("{} = std::move(val);", Naming().FieldNameInCpp(St::LuaTable_TableField));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::ProcessStructDefinition(Class &cls,
                                                const StructDefinition &structDefinition,
                                                bool isFuncTable) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin), isFuncTable);
  }
  for (auto &func: structDefinition.mFunctions) {
    if (isFuncTable || func.GetAnnotation(Annotations::LuaFunc))
      ProcessLuaFunction(cls, func, isFuncTable);
  }
}

void LuaFunctionPlugin::ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition,
                                           bool isFuncTable) {
  cls.mSourceIncludes.AddLibHeader("lua.hpp");
  cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
  cls.mHeaderIncludes.AddForwardDeclaration({"", "struct", "lua_State"});
  const std::string *sourceTable = nullptr;
  if (isFuncTable) {
    auto attrib = cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)
                      ->GetAttribute(Annotations::LuaFuncTable_SourceTable);
    if (attrib) {
      sourceTable = &attrib->mValue.mName;
    }

    bool isStatic = cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)
                        ->GetAttribute(Annotations::LuaFuncTable_Static);

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
    GenerateFunction(cls, functionDefinition, sourceTable, field.mName, isFuncTable, false);
    GenerateFunctionSetter(cls, functionDefinition, field);
    GenerateFunctionChecker(cls, functionDefinition, field);
    Validate().NewField(cls, field);
    cls.mFields.push_back(std::move(field));
  }
}

void LuaFunctionPlugin::GenerateFunctionPushArgs(ClassMethod &method,
                                                 const FunctionDefinition &functionDefinition) {
  for (auto &funcArg: functionDefinition.mArguments) {
    method.mBody.Add("{}::{}({}, luaState);", St::LuaHelper, St::LuaHelper_Push, funcArg.mName);
  }
}

void LuaFunctionPlugin::GenerateFunctionChecker(Class &cls,
                                                const FunctionDefinition &functionDefinition,
                                                ClassField &functionHandle) {
  auto method = ClassMethod{Naming().LuaFunctionCheckerNameInCpp(functionDefinition), Type{"bool"},
                            Visibility::Public, Constness::Const};
  method.mBody.Add("return !{}.empty();", functionHandle.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::GenerateFunctionSetter(Class &cls,
                                               const FunctionDefinition &functionDefinition,
                                               ClassField &functionHandle) {
  auto method = ClassMethod{Naming().LuaFunctionSetterNameInCpp(functionDefinition), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("val", Type{"std::string"});
  method.mBody.Add("{} = val;", functionHandle.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaFunctionPlugin::GenerateFunctionGetFunctionFromSourceTable(
    const FunctionDefinition &functionDefinition, const std::string *sourceTable,
    const std::string &functionHandle, bool isFuncTable, ClassMethod &method,
    const std::string &retVal) {
  method.mBody.Add("lua_getglobal(luaState, \"{}\");", *sourceTable);
  if (isFuncTable) {
    method.mBody.Add("lua_pushstring(luaState, {}.c_str());",
                     Naming().FieldNameInCpp(St::LuaTable_TableField));
    method.mBody.Add("lua_gettable(luaState, -2);");
    method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
    method.mBody.Indent(1);
    method.mBody.Add("HOLGEN_WARN(\"Function table {}.{{}} not found when calling {}\", {});",
                     *sourceTable, functionDefinition.mName,
                     Naming().FieldNameInCpp(St::LuaTable_TableField));
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
  if (isFuncTable) {
    method.mBody.Add("HOLGEN_WARN(\"Calling undefined {} function in {}.{{}}\", {});",
                     functionDefinition.mName, *sourceTable,
                     Naming().FieldNameInCpp(St::LuaTable_TableField));
  } else {
    method.mBody.Add("HOLGEN_WARN(\"Calling undefined {} function {}.{{}}\", {});",
                     functionDefinition.mName, *sourceTable, functionHandle);
  }
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return {};", retVal);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void LuaFunctionPlugin::GenerateFunctionGetGlobalFunction(
    const FunctionDefinition &functionDefinition, const std::string &functionHandle,
    bool isFuncTable, ClassMethod &method, const std::string &retVal) {
  if (isFuncTable) {
    method.mBody.Add("lua_getglobal(luaState, {}.c_str());",
                     Naming().FieldNameInCpp(St::LuaTable_TableField));

    method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
    method.mBody.Indent(1);
    method.mBody.Add("HOLGEN_WARN(\"Function table {{}} not found when calling {}\", {});",
                     functionDefinition.mName, Naming().FieldNameInCpp(St::LuaTable_TableField));
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
  if (isFuncTable) {
    method.mBody.Add("HOLGEN_WARN(\"Calling undefined {} function in {{}}\", {});",
                     functionDefinition.mName, Naming().FieldNameInCpp(St::LuaTable_TableField));
  } else {
    method.mBody.Add("HOLGEN_WARN(\"Calling undefined {} function {{}}\", {});",
                     functionDefinition.mName, functionHandle);
  }
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return {};", retVal);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
}

void LuaFunctionPlugin::GenerateFunction(Class &cls, const FunctionDefinition &functionDefinition,
                                         const std::string *sourceTable,
                                         const std::string &functionHandle, bool isFuncTable,
                                         bool isStatic) {
  auto method = NewFunction(functionDefinition);
  method.mConstness = Constness::Const;
  method.mFunction = &functionDefinition;
  method.mArguments.emplace_front("luaState", Type{"lua_State", PassByType::Pointer});
  std::string retVal = "{}";
  if (functionDefinition.mReturnType.mType.mName == "void")
    retVal = "void()";

  if (isFuncTable) {
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {}, "Calling unset {} function from table");)",
        Naming().FieldNameInCpp(St::LuaTable_TableField), retVal, functionDefinition.mName);
  } else {
    method.mBody.Add(R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {}, "Calling unset {} function");)",
                     functionHandle, retVal, functionDefinition.mName);
  }

  if (sourceTable) {
    GenerateFunctionGetFunctionFromSourceTable(functionDefinition, sourceTable, functionHandle,
                                               isFuncTable, method, retVal);
  } else {
    GenerateFunctionGetGlobalFunction(functionDefinition, functionHandle, isFuncTable, method,
                                      retVal);
  }

  if (!isStatic) {
    method.mBody.Add("{}::{}(*this, luaState);", St::LuaHelper, St::LuaHelper_Push);
  }
  GenerateFunctionPushArgs(method, functionDefinition);

  bool returnsVal = method.mReturnType.mName != "void";
  method.mBody.Add("lua_call(luaState, {}, {});", functionDefinition.mArguments.size() + !isStatic,
                   returnsVal ? 1 : 0);
  int popCount = returnsVal + isFuncTable + !!sourceTable;
  if (returnsVal) {
    if (mProject.GetClass(method.mReturnType.mName)) {
      method.mReturnType.mType = PassByType::Pointer;
      method.mBody.Add("auto result = {}::ReadFromLua(luaState, -1);", method.mReturnType.mName);
    } else {
      method.mBody.Add("{} result;", method.mReturnType.mName);
      method.mBody.Add("{}::{}(result, luaState, -1);", St::LuaHelper, St::LuaHelper_Read);
    }
    if (popCount > 0) {
      method.mBody.Add("lua_pop(luaState, {});", popCount);
    }
    method.mBody.Add("return result;");
  } else {
    if (popCount > 0) {
      method.mBody.Add("lua_pop(luaState, {});", popCount);
    }
  }
  FillComments(functionDefinition, method.mComments);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
