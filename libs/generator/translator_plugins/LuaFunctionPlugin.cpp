#include "LuaFunctionPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void LuaFunctionPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      if (cls.mStruct->GetAnnotation(Annotations::NoLua))
        return;
      for (auto &func: cls.mStruct->mFunctions) {
        if (func.GetAnnotation(Annotations::LuaFunc))
          AddLuaFunction(cls, func);
      }
    }
  }

  void LuaFunctionPlugin::AddLuaFunction(Class &cls, const FunctionDefinition &func) {
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
    cls.mGlobalForwardDeclarations.insert("struct lua_State;");
    auto &field = cls.mFields.emplace_back("mFuncName_" + func.mName, Type{"std::string"});

    {
      auto &setter = cls.mMethods.emplace_back(
          St::GetSetterMethodName(func.mName) + "LuaFunc", Type{"void"},
          Visibility::Public, Constness::NotConst);
      setter.mArguments.emplace_back("val", Type{"std::string"});
      setter.mBody.Add("{} = val;", field.mName);
    }
    auto &method = cls.mMethods.emplace_back(
        St::Capitalize(func.mName),
        Type{func.mReturnType}
    );
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_RETURN_IF({}.empty(), {{}}, "Calling unset {} function");)",
        field.mName, func.mName);
    auto luaFuncTable = func.GetAnnotation(Annotations::LuaFunc)->GetAttribute(Annotations::LuaFuncTable);

    if (luaFuncTable) {
      method.mBody.Add("lua_getglobal(luaState, \"{}\");", luaFuncTable->mValue.mName);
      method.mBody.Add("lua_pushstring(luaState, {}.c_str());", field.mName);
      method.mBody.Add("lua_gettable(luaState, -2);");
      method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
      method.mBody.Indent(1);
      method.mBody.Add(
          "HOLGEN_WARN(\"Calling undefined {} function {}.{{}}\", {});",
          func.mName, luaFuncTable->mValue.mName, field.mName);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return {{}};");
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    } else {
      method.mBody.Add("lua_getglobal(luaState, {}.c_str());", field.mName);
      method.mBody.Add("if (lua_isnil(luaState, -1)) {{");
      method.mBody.Indent(1);
      method.mBody.Add(
          "HOLGEN_WARN(\"Calling undefined {} function {{}}\", {});",
          func.mName, field.mName);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return {{}};");
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    }

    method.mBody.Add("{}::{}(*this, luaState);", St::LuaHelper, St::LuaHelper_Push);
    AddLuaFunctionPushArgs(cls, method, func);

    bool returnsVal = method.mReturnType.mName != "void";
    method.mBody.Add("lua_call(luaState, {}, {});", 1 + func.mArguments.size(), returnsVal ? 1 : 0);
    if (returnsVal) {
      bool returnsClass = mProject.GetClass(method.mReturnType.mName) != nullptr;
      if (returnsClass) {
        method.mReturnType.mType = PassByType::Pointer;
        method.mBody.Add("{}* result;", method.mReturnType.mName);
        method.mBody.Add("result = {}::ReadFromLua(luaState, -1);", method.mReturnType.mName);
      } else {
        method.mBody.Add("{} result;", method.mReturnType.mName);
        method.mBody.Add("{}::{}(result, luaState, -1);", St::LuaHelper, St::LuaHelper_Read);
      }
      if (luaFuncTable)
        method.mBody.Add("lua_pop(luaState, 2);", field.mName);
      else
        method.mBody.Add("lua_pop(luaState, 1);", field.mName);
      method.mBody.Add("return result;");
    } else {
      if (luaFuncTable)
        method.mBody.Add(R"R(lua_pop(luaState, 1);)R", field.mName);
    }
  }

  void LuaFunctionPlugin::AddLuaFunctionPushArgs(Class &cls __attribute__((unused)), ClassMethod &method,
                                                 const FunctionDefinition &func) {
    for (auto &funcArg: func.mArguments) {
      auto &arg = method.mArguments.emplace_back(funcArg.mName, Type{funcArg.mType});
      if (mProject.GetClass(arg.mType.mName) != nullptr) {
        arg.mType.mType = PassByType::Pointer;
        arg.mType.mConstness = Constness::Const;
      } else if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
        arg.mType.mType = PassByType::Reference;
        arg.mType.mConstness = Constness::Const;
      }
      method.mBody.Add("{}::{}({}, luaState);", St::LuaHelper, St::LuaHelper_Push, arg.mName);
    }

  }
}
