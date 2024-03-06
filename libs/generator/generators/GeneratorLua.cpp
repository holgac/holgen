#include "GeneratorLua.h"
#include "core/Decorators.h"
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

  }

  void GeneratorLua::EnrichClasses() {
    for (auto &cls: mTranslatedProject.mClasses) {
      const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
      if (structDefinition.GetDecorator(Decorators::NoLua))
        continue;

      cls.mGlobalForwardDeclarations.Add("struct lua_State;");
      cls.mSourceIncludes.AddLibHeader("lua.hpp");
      cls.mSourceIncludes.AddLocalHeader("LuaHelper.h");

      auto &pushToLua = cls.mMethods.emplace_back();
      pushToLua.mName = "PushToLua";
      pushToLua.mReturnType.mName = "void";
      pushToLua.mConstness = Constness::Const;
      pushToLua.mVisibility = Visibility::Public;
      {
        auto &arg = pushToLua.mArguments.emplace_back();
        arg.mType.mName = "lua_State";
        arg.mName = "luaState";
        arg.mType.mType = PassByType::Pointer;
      }
      pushToLua.mBody.Line() << "lua_newtable(luaState);";
      auto managedDecorator = structDefinition.GetDecorator(Decorators::Managed);
      if (managedDecorator == nullptr) {
        // TODO: debug generator? Or generate extra debug stuff that's gated behind a macro?
        // Specifying object type would help here
        pushToLua.mBody.Line() << "lua_pushstring(luaState, \"p\");";
        pushToLua.mBody.Line() << "lua_pushlightuserdata(luaState, (void*)this);";
      } else {
        auto idField = cls.GetField(St::GetFieldNameInCpp(structDefinition.GetIdField()->mName));
        std::string tempType = "uint64_t";
        if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
          tempType = "int64_t";
        }
        pushToLua.mBody.Line() << tempType << " id = " << idField->mName << ";";
        // TODO: "p" for ptr, "i" for id. Use consts instead of hard-coding them.
        pushToLua.mBody.Line() << "lua_pushstring(luaState, \"i\");";
        pushToLua.mBody.Line() << "lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));";
      }
      pushToLua.mBody.Line() << "lua_settable(luaState, -3);";
      // Do this last so that metamethods don't get called during object construction
      pushToLua.mBody.Line() << "lua_getglobal(luaState, \"" << cls.mName << "Meta\");";
      pushToLua.mBody.Line() << "lua_setmetatable(luaState, -2);";

      auto &createLuaMetatable = cls.mMethods.emplace_back();
      createLuaMetatable.mName = "CreateLuaMetatable";
      createLuaMetatable.mReturnType.mName = "void";
      createLuaMetatable.mIsStatic = true;
      createLuaMetatable.mConstness = Constness::NotConst;
      createLuaMetatable.mVisibility = Visibility::Public;
      {
        auto &arg = createLuaMetatable.mArguments.emplace_back();
        arg.mType.mName = "lua_State";
        arg.mName = "luaState";
        arg.mType.mType = PassByType::Pointer;
      }
      createLuaMetatable.mBody.Line() << "lua_newtable(luaState);";
      CreateIndexMetaMethod(createLuaMetatable.mBody, cls);
      CreateNewIndexMetaMethod(createLuaMetatable.mBody, cls);

      createLuaMetatable.mBody.Line() << "lua_setglobal(luaState, \"" << cls.mName << "Meta\");";
    }
  }

  void GeneratorLua::CreateIndexMetaMethod(CodeBlock &codeBlock, Class &cls) {
    const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
    codeBlock.Line() << "lua_pushstring(luaState, \"__index\");";
    codeBlock.Line() << "lua_pushcfunction(luaState, [](lua_State* ls) {";
    codeBlock.Indent(1);
    auto managedDecorator = structDefinition.GetDecorator(Decorators::Managed);
    if (managedDecorator == nullptr) {
      codeBlock.Line() << "lua_pushstring(ls, \"p\");";
      codeBlock.Line() << "lua_gettable(ls, -3);";
      codeBlock.Line() << "auto instance = (" << cls.mName << "*)lua_touserdata(ls, -1);";
    } else {
      auto manager = mProjectDefinition.GetStruct(managedDecorator->GetAttribute(Decorators::Managed_By)->mValue.mName);
      codeBlock.Line() << "lua_pushstring(ls, \"i\");";
      codeBlock.Line() << "lua_gettable(ls, -3);";
      auto idField = cls.GetField(St::GetFieldNameInCpp(structDefinition.GetIdField()->mName));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      codeBlock.Add("{} id = reinterpret_cast<{}>(lua_touserdata(ls, -1));", idField->mType.mName, tempType);
      auto managerField = manager->GetField(managedDecorator->GetAttribute(Decorators::Managed_Field)->mValue.mName);
      codeBlock.Add(
          "auto instance = {}<{}>::GetInstance()->{}(id);",
          St::GlobalPointer, manager->mName,
          St::GetGetterMethodName(managerField->GetDecorator(Decorators::Container)->GetAttribute(Decorators::Container_ElemName)->mValue.mName));
    }
    codeBlock.Line() << "const char* key = lua_tostring(ls, -2);";
    bool isFirst = true;
    for (auto &fieldDefinition: structDefinition.mFields) {
      if (fieldDefinition.GetDecorator(Decorators::NoLua))
        continue;
      if (isFirst) {
        codeBlock.Line() << "if (0 == strcmp(\"" << fieldDefinition.mName << "\", key)) {";
        isFirst = false;
      } else {
        codeBlock.Line() << "} else if (0 == strcmp(\"" << fieldDefinition.mName << "\", key)) {";
      }
      codeBlock.Indent(1);
      codeBlock.Line() << "LuaHelper::Push(instance->" << St::GetFieldNameInCpp(fieldDefinition.mName) << ", ls);";
      codeBlock.Indent(-1);
    }
    codeBlock.Line() << "} else {";
    codeBlock.Indent(1);
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 1;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __index

    codeBlock.Line() << "lua_settable(luaState, -3);";

  }

  void GeneratorLua::CreateNewIndexMetaMethod(CodeBlock &codeBlock, Class &cls) {
    const auto &structDefinition = *mProjectDefinition.GetStruct(cls.mName);
    codeBlock.Line() << "lua_pushstring(luaState, \"__newindex\");";
    codeBlock.Line() << "lua_pushcfunction(luaState, [](lua_State* ls) {";
    codeBlock.Indent(1);
    codeBlock.Line() << "lua_pushstring(ls, \"p\");";
    codeBlock.Line() << "lua_gettable(ls, -4);";
    codeBlock.Line() << "auto instance = (" << cls.mName << "*)lua_touserdata(ls, -1);";
    codeBlock.Line() << "const char* key = lua_tostring(ls, -3);";
    bool isFirst = true;
    for (auto &fieldDefinition: structDefinition.mFields) {
      // TODO: This can be a bit more nuanced, maybe allow getting but not setting?
      if (fieldDefinition.GetDecorator(Decorators::NoLua))
        continue;
      if (isFirst) {
        codeBlock.Line() << "if (0 == strcmp(\"" << fieldDefinition.mName << "\", key)) {";
        isFirst = false;
      } else {
        codeBlock.Line() << "} else if (0 == strcmp(\"" << fieldDefinition.mName << "\", key)) {";
      }
      codeBlock.Indent(1);
      // TODO: This appends to containers, so a=[1] a=[2] results in a=[1,2].
      codeBlock.Line() << "LuaHelper::Read(instance->" << St::GetFieldNameInCpp(fieldDefinition.mName) << ", ls, -2);";
      codeBlock.Indent(-1);
    }
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __newindex

    codeBlock.Line() << "lua_settable(luaState, -3);";

  }

  void GeneratorLua::GenerateHelpers() {
    GenerateLuaHelper(mTranslatedProject.mClasses.emplace_back());
  }

  void GeneratorLua::GenerateLuaHelper(Class &generatedClass) {
    generatedClass.mName = "LuaHelper";
    generatedClass.mGlobalForwardDeclarations.Add("struct lua_State;");
    generatedClass.mSourceIncludes.AddLibHeader("lua.hpp");
    GenerateLuaHelperPush(generatedClass);
    GenerateLuaHelperRead(generatedClass);
  }

  void GeneratorLua::GenerateLuaHelperPush(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back();
    baseFunc.mName = "Push";
    baseFunc.mConstness = Constness::NotConst;
    baseFunc.mIsStatic = true;
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

    for (const auto &[type, usage]: LuaUsage) {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = "Push";
      func.mConstness = Constness::NotConst;
      func.mIsStatic = true;
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
      func.mIsStatic = true;
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
      func.mIsStatic = true;
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

  void GeneratorLua::GenerateLuaHelperRead(Class &generatedClass) {
    auto &baseFunc = generatedClass.mMethods.emplace_back();
    baseFunc.mName = "Read";
    baseFunc.mConstness = Constness::NotConst;
    baseFunc.mIsStatic = true;
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
      func.mIsStatic = true;
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
      func.mIsStatic = true;
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
      func.mIsStatic = true;
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