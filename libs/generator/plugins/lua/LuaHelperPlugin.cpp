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
    auto &generatedClass = mProject.mClasses.emplace_back(St::LuaHelper);
    generatedClass.mHeaderIncludes.AddLibHeader("lua.hpp");
    GenerateLuaHelperPush(generatedClass);
    GenerateLuaHelperRead(generatedClass);
    GenerateCreateMetatables(generatedClass);
  }

  void LuaHelperPlugin::GenerateLuaHelperPush(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back(
        "Push", Type{"void"},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    auto &baseTemplateArg = baseFunc.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    baseFunc.mArguments.emplace_back("data", Type{"T", PassByType::Reference, Constness::Const});
    baseFunc.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

    baseFunc.mBody.Add("if constexpr(std::is_pointer_v<T>) {{");
    baseFunc.mBody.Indent(1);
    baseFunc.mBody.Add("if (data) {{");
    baseFunc.mBody.Indent(1);
    baseFunc.mBody.Add("{}::{}(*data, luaState);", St::LuaHelper, St::LuaHelper_Push);
    baseFunc.mBody.Indent(-1);
    baseFunc.mBody.Add("}} else {{");
    baseFunc.mBody.Indent(1);
    baseFunc.mBody.Line() << "lua_pushnil(luaState);";
    baseFunc.mBody.Indent(-1);
    baseFunc.mBody.Add("}}");
    baseFunc.mBody.Indent(-1);
    baseFunc.mBody.Add("}} else {{");
    baseFunc.mBody.Indent(1);
    baseFunc.mBody.Line() << "data.PushToLua(luaState);";
    baseFunc.mBody.Indent(-1);
    baseFunc.mBody.Add("}}");

    GenerateLuaHelperPushNil(generatedClass);

    for (const auto &[type, usage]: LuaUsage) {
      auto &func = generatedClass.mMethods.emplace_back(
          "Push", Type{"void"},
          Visibility::Public, Constness::NotConst, Staticness::Static);

      {
        auto &data = func.mArguments.emplace_back("data", Type{type});
        data.mType.PreventCopying();
      }
      func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

      func.mBody.Line() << usage.mPusher << "(luaState, data" << usage.mFieldExtra << ");";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      GeneratePushForSingleElemContainer(generatedClass, container);
    }

    for (const auto &container: TypeInfo::Get().CppSets) {
       GeneratePushForSingleElemContainer(generatedClass, container);
    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &func = generatedClass.mMethods.emplace_back(
          "Push", Type{"void"},
          Visibility::Public, Constness::NotConst, Staticness::Static
      );
      auto &keyTemplateArg = func.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = func.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";

      {
        auto &data = func.mArguments.emplace_back("data", Type{container, PassByType::Reference});
        data.mType.mTemplateParameters.emplace_back("K");
        data.mType.mTemplateParameters.emplace_back("V");
      }
      func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
      // TODO: implement
    }
  }

  void LuaHelperPlugin::GenerateLuaHelperPushNil(Class &generatedClass) {
    auto &func = generatedClass.mMethods.emplace_back(
        "Push", Type{"void"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    );
    func.mArguments.emplace_back("", Type{"nullptr_t"});
    func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    func.mBody.Line() << "lua_pushnil(luaState);";
  }

  void LuaHelperPlugin::GenerateLuaHelperRead(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back(
        "Read", Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    );
    auto &baseTemplateArg = baseFunc.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    baseFunc.mArguments.emplace_back("data", Type{"T", PassByType::Reference});
    baseFunc.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    baseFunc.mArguments.emplace_back("luaIndex", Type{"int32_t"});

    // TODO: implement reading objects from lua?
    baseFunc.mBody.Line() << "// *data = T::ReadFromLua(luaState, luaIndex);";
    baseFunc.mBody.Line() << "return false; //*data != nullptr;";

    for (const auto &[type, usage]: LuaUsage) {
      auto &func = generatedClass.mMethods.emplace_back(
          "Read", Type{"bool"},
          Visibility::Public, Constness::NotConst, Staticness::Static
      );

      func.mArguments.emplace_back("data", Type{type, PassByType::Reference});
      func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
      func.mArguments.emplace_back("luaIndex", Type{"int32_t"});

      func.mBody.Line() << "if (!" << usage.mValidator << "(luaState, luaIndex))";
      func.mBody.Indent(1);
      func.mBody.Line() << "return false;";
      func.mBody.Indent(-1);
      func.mBody.Line() << "data = " << usage.mGetter << "(luaState, luaIndex);";
      func.mBody.Line() << "return true;";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      GenerateReadForSingleElemContainer(generatedClass, container);
    }

    for (const auto &container: TypeInfo::Get().CppSets) {
      GenerateReadForSingleElemContainer(generatedClass, container);
    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &func = generatedClass.mMethods.emplace_back(
          "Read", Type{"bool"},
          Visibility::Public, Constness::NotConst, Staticness::Static
      );
      auto &keyTemplateArg = func.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = func.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";

      {
        auto &data = func.mArguments.emplace_back("data", Type{container, PassByType::Reference, Constness::Const});
        data.mType.mTemplateParameters.emplace_back("K");
        data.mType.mTemplateParameters.emplace_back("V");
      }
      func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
      func.mArguments.emplace_back("luaIndex", Type{"int32_t"});
      func.mBody.Add("return false;");
      // TODO: implement
    }
  }

  void LuaHelperPlugin::GenerateCreateMetatables(Class &generatedClass) {
    auto &method = generatedClass.mMethods.emplace_back(
        "CreateMetatables", Type{"void"}, Visibility::Public,
        Constness::NotConst, Staticness::Static);
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    for (auto &cls: mProject.mClasses) {
      if (cls.GetMethod("CreateLuaMetatable", Constness::NotConst)) {
        generatedClass.mSourceIncludes.AddLocalHeader(cls.mName + ".h");
        method.mBody.Add("{}::CreateLuaMetatable(luaState);", cls.mName);
      }
    }
  }

  void LuaHelperPlugin::GeneratePushForSingleElemContainer(Class &cls, const std::string &container) {
    auto &func = cls.mMethods.emplace_back(
        "Push", Type{"void"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    );
    auto &templateArg = func.mTemplateParameters.emplace_back();
    templateArg.mType = "typename";
    templateArg.mName = "T";

    {
      auto &data = func.mArguments.emplace_back("data", Type{container, PassByType::Reference});
      data.mType.mTemplateParameters.emplace_back("T");
    }
    func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    // TOOD: implement with rawseti
  }

  void LuaHelperPlugin::GenerateReadForSingleElemContainer(Class &cls, const std::string &container) {
    auto &func = cls.mMethods.emplace_back(
        "Read", Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    );
    auto &templateArg = func.mTemplateParameters.emplace_back();
    templateArg.mType = "typename";
    templateArg.mName = "T";
    {
      auto &data = func.mArguments.emplace_back("data", Type{container, PassByType::Reference, Constness::Const});
      data.mType.mTemplateParameters.emplace_back("T");
    }
    func.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    func.mArguments.emplace_back("luaIndex", Type{"int32_t"});
    // TOOD: implement with rawseti
    func.mBody.Add("return false;");
  }
}