#include "LuaHelperPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {

  namespace {

    struct LuaTypeUsage {
      std::string mValidator;
      std::string mGetter;
      std::string mPusher;
      std::string mFieldExtra;
    };

    std::map<std::string, LuaTypeUsage> LuaUsage = {
        {"int8_t",      {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int16_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int32_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int64_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint8_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint16_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint32_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint64_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"float",       {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"double",      {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"bool",        {"lua_isboolean", "lua_toboolean", "lua_pushboolean", ""}},
        {"std::string", {"lua_isstring",  "lua_tostring",  "lua_pushstring",  ".c_str()"}},
    };

    std::string LuaTableField_Pointer = "p";
    std::string LuaTableField_Index = "i";

  }

  void LuaHelperPlugin::Run() {
    GenerateLuaHelper(mProject.mClasses.emplace_back());
  }

  void LuaHelperPlugin::GenerateLuaHelper(Class &generatedClass) {
    generatedClass.mName = St::LuaHelper;
    generatedClass.mGlobalForwardDeclarations.Add("struct lua_State;");
    generatedClass.mSourceIncludes.AddLibHeader("lua.hpp");
    GenerateLuaHelperPush(generatedClass);
    GenerateLuaHelperRead(generatedClass);
  }

  void LuaHelperPlugin::GenerateLuaHelperPush(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back();
    baseFunc.mName = "Push";
    baseFunc.mConstness = Constness::NotConst;
    baseFunc.mStaticness = Staticness::Static;
    baseFunc.mReturnType.mName = "void";
    auto &baseTemplateArg = baseFunc.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    {
      auto &data = baseFunc.mArguments.emplace_back();
      data.mName = "data";
      data.mType.mName = "T";
      data.mType.mConstness = Constness::Const;
      data.mType.mType = PassByType::Reference;
    }

    {
      auto &luaState = baseFunc.mArguments.emplace_back();
      luaState.mName = "luaState";
      luaState.mType.mName = "lua_State";
      luaState.mType.mType = PassByType::Pointer;
    }

    baseFunc.mBody.Line() << "data.PushToLua(luaState);";

    {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = Constness::NotConst;
      func.mStaticness = Staticness::Static;
      func.mReturnType.mName = "void";

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "";
        data.mType.mName = "nullptr_t";
      }

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mType.mName = "lua_State";
        luaState.mType.mType = PassByType::Pointer;
      }

      func.mBody.Line() << "lua_pushnil(luaState);";
    }

    for (const auto &[type, usage]: LuaUsage) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = Constness::NotConst;
      func.mStaticness = Staticness::Static;
      func.mReturnType.mName = "void";

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mType.mName = type;
        if (!TypeInfo::Get().CppPrimitives.contains(type)) {
          data.mType.mConstness = Constness::Const;
          // func.mIsTemplateSpecialization = true;
          data.mType.mType = PassByType::Reference;
        }
      }

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mType.mName = "lua_State";
        luaState.mType.mType = PassByType::Pointer;
      }

      func.mBody.Line() << usage.mPusher << "(luaState, data" << usage.mFieldExtra << ");";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = Constness::NotConst;
      func.mStaticness = Staticness::Static;
      auto &templateArg = func.mTemplateParameters.emplace_back();
      templateArg.mType = "typename";
      templateArg.mName = "T";
      func.mReturnType.mName = "void";

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mType.mName = container;
        auto &outTemplate = data.mType.mTemplateParameters.emplace_back();
        outTemplate.mName = "T";
        data.mType.mType = PassByType::Reference;
      }

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mType.mName = "lua_State";
        luaState.mType.mType = PassByType::Pointer;
      }

      // TOOD: implement with rawseti

    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = Constness::NotConst;
      func.mStaticness = Staticness::Static;
      auto &keyTemplateArg = func.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = func.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";
      func.mReturnType.mName = "void";

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mType.mName = container;
        auto &keyTemplate = data.mType.mTemplateParameters.emplace_back();
        keyTemplate.mName = "K";
        auto &valueTemplate = data.mType.mTemplateParameters.emplace_back();
        valueTemplate.mName = "V";
        data.mType.mType = PassByType::Reference;
      }

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mType.mName = "lua_State";
        luaState.mType.mType = PassByType::Pointer;
      }
      // TODO: implement

    }
  }

  void LuaHelperPlugin::GenerateLuaHelperRead(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back();
    baseFunc.mName = "Read";
    baseFunc.mConstness = Constness::NotConst;
    baseFunc.mStaticness = Staticness::Static;
    baseFunc.mReturnType.mName = "bool";
    auto &baseTemplateArg = baseFunc.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    {
      auto &data = baseFunc.mArguments.emplace_back();
      data.mName = "data";
      data.mType.mName = "T";
      data.mType.mConstness = Constness::NotConst;
      data.mType.mType = PassByType::Reference;
    }

    {
      auto &luaState = baseFunc.mArguments.emplace_back();
      luaState.mName = "luaState";
      luaState.mType.mName = "lua_State";
      luaState.mType.mType = PassByType::Pointer;
    }

    {
      auto &luaIdx = baseFunc.mArguments.emplace_back();
      luaIdx.mName = "luaIndex";
      luaIdx.mType.mName = "int32_t";
    }

    // TODO: implement reading objects from lua?
    baseFunc.mBody.Line() << "// return data.ReadFromLua(luaState, luaIndex);";
    baseFunc.mBody.Line() << "return false;";

    for (const auto &[type, usage]: LuaUsage) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Read";
      func.mConstness = Constness::NotConst;
      func.mStaticness = Staticness::Static;
      func.mReturnType.mName = "bool";

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mType.mName = type;
        data.mType.mType = PassByType::Reference;
        // func.mIsTemplateSpecialization = true;
      }

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mType.mName = "lua_State";
        luaState.mType.mType = PassByType::Pointer;
      }

      {
        auto &luaIdx = func.mArguments.emplace_back();
        luaIdx.mName = "luaIndex";
        luaIdx.mType.mName = "int32_t";
      }

      func.mBody.Line() << "if (!" << usage.mValidator << "(luaState, luaIndex))";
      func.mBody.Indent(1);
      func.mBody.Line() << "return false;";
      func.mBody.Indent(-1);
      func.mBody.Line() << "data = " << usage.mGetter << "(luaState, luaIndex);";
      func.mBody.Line() << "return true;";
    }

    /*
    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = false;
      func.mStaticness = Staticness::Static;
      auto &templateArg = func.mTemplateParameters.emplace_back();
      templateArg.mReturnType = "typename";
      templateArg.mName = "T";
      func.mReturnType.mName = "void";

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mReturnType.mName = "lua_State";
        luaState.mReturnType.mReturnType = PassByType::Pointer;
      }

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mReturnType.mName = container;
        auto &outTemplate = data.mReturnType.mTemplateParameters.emplace_back();
        outTemplate.mName = "T";
        data.mReturnType.mReturnType = PassByType::Reference;
      }

      // TOOD: implement with rawseti

    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = false;
      func.mStaticness = Staticness::Static;
      auto &keyTemplateArg = func.mTemplateParameters.emplace_back();
      keyTemplateArg.mReturnType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = func.mTemplateParameters.emplace_back();
      valueTemplateArg.mReturnType = "typename";
      valueTemplateArg.mName = "V";
      func.mReturnType.mName = "void";

      {
        auto &luaState = func.mArguments.emplace_back();
        luaState.mName = "luaState";
        luaState.mReturnType.mName = "lua_State";
        luaState.mReturnType.mReturnType = PassByType::Pointer;
      }

      {
        auto &data = func.mArguments.emplace_back();
        data.mName = "data";
        data.mReturnType.mName = container;
        auto &keyTemplate = data.mReturnType.mTemplateParameters.emplace_back();
        keyTemplate.mName = "K";
        auto &valueTemplate = data.mReturnType.mTemplateParameters.emplace_back();
        valueTemplate.mName = "V";
        data.mReturnType.mReturnType = PassByType::Reference;
      }
      // TODO: implement

    }
     */
  }
}