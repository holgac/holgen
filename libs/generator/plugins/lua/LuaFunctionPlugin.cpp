#include "LuaFunctionPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void LuaFunctionPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr || cls.mStruct->GetAnnotation(Annotations::NoLua))
        continue;
      ProcessStructDefinition(cls, *cls.mStruct);
    }
  }

  void LuaFunctionPlugin::ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition) {
    for(auto& mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
    }
    for (auto &func: structDefinition.mFunctions) {
      if (func.GetAnnotation(Annotations::LuaFunc))
        ProcessLuaFunction(cls, func);
    }
  }

  void LuaFunctionPlugin::ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition) {
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
    cls.mGlobalForwardDeclarations.insert({"struct", "lua_State"});
    auto field = ClassField{Naming().LuaFunctionHandleNameInCpp(functionDefinition), Type{"std::string"}};
    GenerateFunction(cls, functionDefinition, field);
    GenerateFunctionSetter(cls, functionDefinition, field);
    Validate().NewField(cls, field);
    cls.mFields.push_back(std::move(field));
  }

  void LuaFunctionPlugin::GenerateFunctionPushArgs(ClassMethod &method, const FunctionDefinition &functionDefinition) {
    for (auto &funcArg: functionDefinition.mArguments) {
      auto &arg = method.mArguments.emplace_back(funcArg.mName, Type{mProject, funcArg.mType});
      arg.mType.PreventCopying();
      if (mProject.GetClass(arg.mType.mName) != nullptr)
        arg.mType.mType = PassByType::Pointer;
      method.mBody.Add("{}::{}({}, luaState);", St::LuaHelper, St::LuaHelper_Push, arg.mName);
    }
  }

  void LuaFunctionPlugin::GenerateFunctionSetter(Class &cls, const FunctionDefinition &functionDefinition,
                                                 ClassField &functionHandle) {
    auto method = ClassMethod{
        Naming().LuaFunctionSetterNameInCpp(functionDefinition), Type{"void"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"std::string"});
    method.mBody.Add("{} = val;", functionHandle.mName);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaFunctionPlugin::GenerateFunction(
      Class &cls, const FunctionDefinition &functionDefinition, ClassField &functionHandle) {
    auto method = ClassMethod{
        St::Capitalize(functionDefinition.mName),
        Type{mProject, functionDefinition.mReturnType}
    };
    method.mFunction = &functionDefinition;
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    // TODO: this doesn't work if function returns void
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {{}}, "Calling unset {} function");)",
        functionHandle.mName, functionDefinition.mName);
    auto luaFuncTable = functionDefinition.GetAnnotation(Annotations::LuaFunc)->GetAttribute(Annotations::LuaFunc_Table);

    if (luaFuncTable) {
      method.mBody.Add("lua_getglobal(luaState, \"{}\");", luaFuncTable->mValue.mName);
      method.mBody.Add("lua_pushstring(luaState, {}.c_str());", functionHandle.mName);
      method.mBody.Add("lua_gettable(luaState, -2);");
      method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
      method.mBody.Indent(1);
      method.mBody.Add(
          "HOLGEN_WARN(\"Calling undefined {} function {}.{{}}\", {});",
          functionDefinition.mName, luaFuncTable->mValue.mName, functionHandle.mName);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return {{}};");
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    } else {
      method.mBody.Add("lua_getglobal(luaState, {}.c_str());", functionHandle.mName);
      method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
      method.mBody.Indent(1);
      method.mBody.Add(
          "HOLGEN_WARN(\"Calling undefined {} function {{}}\", {});",
          functionDefinition.mName, functionHandle.mName);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return {{}};");
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    }

    method.mBody.Add("{}::{}(*this, luaState);", St::LuaHelper, St::LuaHelper_Push);
    GenerateFunctionPushArgs(method, functionDefinition);

    bool returnsVal = method.mReturnType.mName != "void";
    method.mBody.Add("lua_call(luaState, {}, {});", 1 + functionDefinition.mArguments.size(), returnsVal ? 1 : 0);
    if (returnsVal) {
      if (mProject.GetClass(method.mReturnType.mName)) {
        method.mReturnType.mType = PassByType::Pointer;
        method.mBody.Add("auto result = {}::ReadFromLua(luaState, -1);", method.mReturnType.mName);
      } else {
        method.mBody.Add("{} result;", method.mReturnType.mName);
        method.mBody.Add("{}::{}(result, luaState, -1);", St::LuaHelper, St::LuaHelper_Read);
      }
      if (luaFuncTable)
        method.mBody.Add("lua_pop(luaState, 2);", functionHandle.mName);
      else
        method.mBody.Add("lua_pop(luaState, 1);", functionHandle.mName);
      method.mBody.Add("return result;");
    } else {
      if (luaFuncTable)
        method.mBody.Add(R"R(lua_pop(luaState, 1);)R", functionHandle.mName);
    }
    FillComments(functionDefinition, method.mComments);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}
