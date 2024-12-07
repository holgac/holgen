#include "LuaPluginBase.h"
#include "core/St.h"

namespace holgen {

void LuaPluginBase::GenerateMethodCaller(Class &cls, const ClassMethod &method,
                                         const std::string &methodName) {
  auto methodCaller = ClassMethod{methodName, Type{"int"}, Visibility::Private, Constness::NotConst,
                                  Staticness::Static};
  methodCaller.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  bool isLuaFuncTable = false;
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    isLuaFuncTable = true;
  }
  bool isLuaFunc = false;
  if (method.mFunction) {
    if (isLuaFuncTable || method.mFunction->GetAnnotation(Annotations::LuaFunc)) {
      isLuaFunc = true;
    }
  }

  std::string callPrefix;
  if (method.mStaticness == Staticness::Static) {
    callPrefix = std::format("{}::", cls.mName);
  } else {
    GenerateInstanceGetter(cls, methodCaller.mBody,
                           -ptrdiff_t(method.mArguments.size()) - 1 + isLuaFunc, "instance");
    methodCaller.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!instance, 0, \"Calling {}.{} method with "
                           "an invalid lua proxy object!\");",
                           cls.mName, method.mName);
    callPrefix = std::format("instance->", cls.mName);
  }
  std::string funcArgs =
      GenerateReadExposedMethodArgsAndGetArgsString(cls, method, methodCaller.mBody, isLuaFunc);

  if (isLuaFunc) {
    if (funcArgs.empty()) {
      funcArgs = "luaState";
    } else {
      funcArgs = std::format("luaState, {}", funcArgs);
    }
  }

  if (method.mReturnType.mName != "void") {
    if (method.mReturnType.mType == PassByType::Reference) {
      methodCaller.mBody.Add("auto& result = {}{}({});", callPrefix, method.mName, funcArgs);
    } else {
      methodCaller.mBody.Add("auto result = {}{}({});", callPrefix, method.mName, funcArgs);
    }
    std::string accessor = ".";
    if (method.mReturnType.mType == PassByType::Pointer) {
      accessor = "->";
    }
    auto returnedClass = mProject.GetClass(method.mReturnType.mName);
    if (returnedClass && !returnedClass->mEnum) {
      bool pushMirror = method.mReturnType.mType == PassByType::Value;
      methodCaller.mBody.Add("{}::{}<{}>(result, luaState);", St::LuaHelper, St::LuaHelper_Push,
                             pushMirror ? "true" : "false");
    } else {
      bool shouldBeMirror = false;
      if (method.mReturnType.mType == PassByType::Value) {
        shouldBeMirror = true;
        if (TypeInfo::Get().CppIndexedContainers.contains(method.mReturnType.mName)) {
          auto &underlyingType = method.mReturnType.mTemplateParameters.front();
          if (underlyingType.mType == PassByType::Pointer) {
            shouldBeMirror = false;
          }
        }
      }
      methodCaller.mBody.Add("{}::{}<{}>(result, luaState);", St::LuaHelper, St::LuaHelper_Push,
                             shouldBeMirror ? "true" : "false");
    }
    methodCaller.mBody.Add("return 1;");
  } else {
    methodCaller.mBody.Add("{}{}({});", callPrefix, method.mName, funcArgs);
    methodCaller.mBody.Add("return 0;");
  }
  Validate().NewMethod(cls, methodCaller);
  cls.mMethods.push_back(std::move(methodCaller));
}

void LuaPluginBase::GenerateInstanceGetter(Class &cls, CodeBlock &codeBlock, int index,
                                           const std::string &outVarName) {
  codeBlock.Add("auto {} = {}::{}(luaState, {});", outVarName, cls.mName, St::Lua_ReadProxyObject,
                index);
}

std::string LuaPluginBase::GenerateReadExposedMethodArgsAndGetArgsString(
    const Class &cls, const ClassMethod &exposedMethod, CodeBlock &switchBlock, bool isLuaFunc) {
  std::stringstream funcArgs;
  size_t i = 0;
  for (auto &arg: exposedMethod.mArguments) {
    if (arg.mType.mName == "lua_State") {
      THROW_IF(i != 0 || !isLuaFunc, "Unexpected lua_State argument!");
      continue;
    }
    if (i != 0)
      funcArgs << ", ";
    ptrdiff_t stackIdx = ptrdiff_t(i) - ptrdiff_t(exposedMethod.mArguments.size()) + isLuaFunc;
    if (auto argClass = mProject.GetClass(arg.mType.mName)) {
      bool canBeMirror = !cls.HasVirtualMethods() && arg.mType.SupportsMirroring(mProject);
      bool canBeProxy = true;
      bool canBeNull = arg.mType.mType == PassByType::Pointer;
      if (arg.mType.mType != PassByType::Value && arg.mType.mConstness == Constness::NotConst) {
        canBeMirror = false;
      }
      if (argClass->mEnum) {
        canBeProxy = false;
      }
      if (canBeProxy && canBeMirror) {
        switchBlock.Add("{} arg{}Mirror;", arg.mType.mName, i);
        switchBlock.Add("{} *arg{};", arg.mType.mName, i);
        if (canBeNull) {
          switchBlock.Add("if (lua_isnil(luaState, {})) {{", stackIdx);
          switchBlock.Indent(1);
          switchBlock.Add("arg{} = nullptr;", i);
          switchBlock.Indent(-1);
          switchBlock.Add("}} else if (lua_getmetatable(luaState, {})) {{", stackIdx);
        } else {
          switchBlock.Add("if (lua_getmetatable(luaState, {})) {{", stackIdx);
        }
        switchBlock.Indent(1);
        switchBlock.Add("lua_pop(luaState, 1);");
        switchBlock.Add("arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadProxyObject, stackIdx);
        switchBlock.Indent(-1);
        switchBlock.Add("}} else {{");
        switchBlock.Indent(1);
        switchBlock.Add("arg{}Mirror = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadMirrorObject, stackIdx);
        switchBlock.Add("arg{0} = &arg{0}Mirror;", i);
        switchBlock.Indent(-1);
        switchBlock.Add("}}");
      } else if (canBeProxy) {
        switchBlock.Add("auto arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadProxyObject, stackIdx);
      } else if (canBeMirror) {
        switchBlock.Add("auto arg{} = {}::{}(luaState, {});", i, arg.mType.mName,
                        St::Lua_ReadMirrorObject, stackIdx);
      } else {
        THROW("Dont know how to pass {}.{} argument", exposedMethod.mName, arg.mName);
      }
      if (canBeProxy) {
        if (arg.mType.mType == PassByType::Pointer) {
          funcArgs << "arg" << i;
        } else {
          funcArgs << "*arg" << i;
        }
      } else {
        if (arg.mType.mType == PassByType::Pointer) {
          funcArgs << "&arg" << i;
        } else {
          funcArgs << "arg" << i;
        }
      }
    } else {
      auto sanitizedType = arg.mType;
      sanitizedType.mType = PassByType::Value;
      sanitizedType.mConstness = Constness::NotConst;
      switchBlock.Add("{}arg{};", sanitizedType.ToString(false), i);
      switchBlock.Add("{}::{}(arg{}, luaState, {});", St::LuaHelper, St::LuaHelper_Read, i,
                      stackIdx);
      funcArgs << "arg" << i;
    }
    ++i;
  }
  return funcArgs.str();
}
} // namespace holgen
