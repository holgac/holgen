#include "LuaPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "../../NamingConvention.h"

namespace holgen {
  namespace {
    std::string LuaTableField_Pointer = "p";
    std::string LuaTableField_Index = "i";
  }

  void LuaPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct)
        ProcessStruct(cls);
      else if (cls.mEnum)
        ProcessEnum(cls);
    }
  }

  void LuaPlugin::GeneratePushIndexMetaMethod(Class &cls) {
    auto method = ClassMethod{"PushIndexMetaMethod", Type{"void"},
                              Visibility::Private, Constness::NotConst, Staticness::Static};
    auto &codeBlock = method.mBody;
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    codeBlock.Add("lua_pushstring(luaState, \"__index\");");
    codeBlock.Add("lua_pushcfunction(luaState, [](lua_State* ls) {{");
    codeBlock.Indent(1);
    codeBlock.Add("auto instance = {}::ReadFromLua(ls, -2);", cls.mName);
    codeBlock.Add("const char* key = lua_tostring(ls, -1);");
    bool isFirst = true;
    for (auto &field: cls.mFields) {
      if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua))
        continue;
      bool isRef = field.mField->mType.mName == "Ref";
      if (isFirst) {
        codeBlock.Add("if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(*field.mField));
        isFirst = false;
      } else {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(*field.mField));
      }
      codeBlock.Indent(1);
      codeBlock.Add("{}::{}(instance->{}, ls);", St::LuaHelper, St::LuaHelper_Push, field.mName);
      codeBlock.Indent(-1);
      if (isRef && field.mType.mType != PassByType::Pointer) {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{",
                      Naming().FieldNameInLua(*field.mField, true));
        codeBlock.Indent(1);
        codeBlock.Add("{}::{}({}::{}(instance->{}), ls);", St::LuaHelper, St::LuaHelper_Push,
                      field.mField->mType.mTemplateParameters[0].mName, St::ManagedObject_Getter, field.mName);
        codeBlock.Indent(-1);
      }
    }
    for (auto &luaMethod: cls.mMethods) {
      if (!luaMethod.mExposeToLua)
        continue;
      if (isFirst) {
        codeBlock.Add("if (0 == strcmp(\"{}\", key)) {{", luaMethod.mName);
        isFirst = false;
      } else {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{", luaMethod.mName);
      }
      codeBlock.Indent(1);
      // TODO: LuaHelper::Push should work with functions
      codeBlock.Add("lua_pushcfunction(ls, [](lua_State* lsInner) {{");
      codeBlock.Indent(1);
      codeBlock.Add("auto instance = {}::ReadFromLua(lsInner, {});", cls.mName,
                    -ssize_t(luaMethod.mArguments.size()) - 1);
      std::stringstream funcArgs;
      size_t i = 0;
      for (auto &arg: luaMethod.mArguments) {
        if (i != 0)
          funcArgs << ", ";
        funcArgs << "arg" << i;
        auto sanitizedType = arg.mType;
        sanitizedType.mType = PassByType::Value;
        sanitizedType.mConstness = Constness::NotConst;
        codeBlock.Add("{} arg{};", sanitizedType.ToString(), i);
        codeBlock.Add(
            "{}::{}(arg{}, lsInner, {});",
            St::LuaHelper, St::LuaHelper_Read, i,
            ssize_t(i) - ssize_t(luaMethod.mArguments.size()));
        ++i;
      }
      if (luaMethod.mReturnType.mName != "void") {
        codeBlock.Add("auto result = instance->{}({});", luaMethod.mName, funcArgs.str());
        codeBlock.Add("{}::{}(result, lsInner);", St::LuaHelper, St::LuaHelper_Push);
        codeBlock.Add("return 1;");
      } else {
        codeBlock.Add("instance->{}({});", luaMethod.mName, funcArgs.str());
        codeBlock.Add("return 0;");
      }
      codeBlock.Indent(-1);
      codeBlock.Add("}});");
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
    codeBlock.Add("lua_settable(luaState, -3);");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::GeneratePushNewIndexMetaMethod(Class &cls) {
    auto method = ClassMethod{
        "PushNewIndexMetaMethod", Type{"void"}, Visibility::Private, Constness::NotConst, Staticness::Static};
    auto &codeBlock = method.mBody;
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    auto structDefinition = cls.mStruct;
    if (structDefinition == nullptr)
      return;
    codeBlock.Add("lua_pushstring(luaState, \"__newindex\");");
    codeBlock.Add("lua_pushcfunction(luaState, [](lua_State* ls) {{");
    codeBlock.Indent(1);

    codeBlock.Add("auto instance = {}::ReadFromLua(ls, -3);", cls.mName);
    codeBlock.Add("const char* key = lua_tostring(ls, -2);");
    bool isFirst = true;
    for (auto &field: cls.mFields) {
      // TODO: This can be a bit more nuanced, maybe allow getting but not setting?
      if (!field.mField || field.mField->GetAnnotation(Annotations::NoLua))
        continue;
      if (isFirst) {
        codeBlock.Add("if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(*field.mField));
        isFirst = false;
      } else {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(*field.mField));
      }
      // TODO: Implement else case
      // TODO: Make this work with pointers
      // TODO: Make this work with nested structs
      codeBlock.Indent(1);
      // TODO: This appends to containers, so a=[1] a=[2] results in a=[1,2].
      codeBlock.Add("{}::{}(instance->{}, ls, -1);", St::LuaHelper, St::LuaHelper_Read, field.mName);
      codeBlock.Indent(-1);
    }
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __newindex
    codeBlock.Add("lua_settable(luaState, -3);");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::GenerateReadFromLua(Class &cls) {
    // Only works with negative indices
    // TODO: generate comments?
    auto method = ClassMethod{
        "ReadFromLua", Type{cls.mName, PassByType::Pointer},
        Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    if (cls.mStruct->GetAnnotation(Annotations::Managed)) {
      method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
      method.mBody.Add("lua_gettable(luaState, idx - 1);");
      auto idField = cls.GetField(Naming().FieldNameInCpp(*cls.mStruct->GetIdField()));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      method.mBody.Add("{} id = reinterpret_cast<{}>(lua_touserdata(luaState, -1));", idField->mType.mName, tempType);
      method.mBody.Add("auto ptr = {}::{}(id);", cls.mName, St::ManagedObject_Getter);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return ptr;");
    } else {
      method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
      method.mBody.Add("lua_gettable(luaState, idx - 1);");
      method.mBody.Add("auto ptr = ({}*)lua_touserdata(luaState, -1);", cls.mName);
      method.mBody.Add("lua_pop(luaState, 1);");
      method.mBody.Add("return ptr;");
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::GeneratePushToLua(Class &cls) {
    auto method = ClassMethod{
        "PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

    method.mBody.Add("lua_newtable(luaState);");
    if (cls.mStruct->GetAnnotation(Annotations::Managed)) {
      auto idField = cls.GetField(Naming().FieldNameInCpp(*cls.mStruct->GetIdField()));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      method.mBody.Add("{} id = {};", tempType, idField->mName);
      method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
      method.mBody.Add("lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));");
    } else {
      // TODO: debug generator? Or generate extra debug stuff that's gated behind a macro?
      // Specifying object type would help here
      method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
      method.mBody.Add("lua_pushlightuserdata(luaState, (void*)this);");
    }
    method.mBody.Add("lua_settable(luaState, -3);");
    // Do this last so that metamethods don't get called during object construction
    method.mBody.Add("lua_getglobal(luaState, \"{}\");", Naming().LuaMetatableName(cls));
    method.mBody.Add("lua_setmetatable(luaState, -2);");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::ProcessStruct(Class &cls) {
    if (cls.mStruct->GetAnnotation(Annotations::NoLua))
      return;
    cls.mGlobalForwardDeclarations.insert({"struct", "lua_State"});
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
    GeneratePushToLua(cls);
    GeneratePushGlobalToLua(cls);
    GenerateReadFromLua(cls);
    GeneratePushIndexMetaMethod(cls);
    GeneratePushNewIndexMetaMethod(cls);
    GenerateCreateLuaMetatable(cls);
  }

  void LuaPlugin::GenerateCreateLuaMetatable(Class &cls) {
    auto method = ClassMethod{
        "CreateLuaMetatable", Type{"void"}, Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mBody.Add("lua_newtable(luaState);");
    method.mBody.Add("PushIndexMetaMethod(luaState);");
    method.mBody.Add("PushNewIndexMetaMethod(luaState);");
    method.mBody.Add("lua_setglobal(luaState, \"{}\");", Naming().LuaMetatableName(cls));
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::ProcessEnum(Class &cls) {
    if (cls.mEnum->GetAnnotation(Annotations::NoLua))
      return;
    cls.mGlobalForwardDeclarations.insert({"struct", "lua_State"});
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mBody.Add("{}::{}(mValue, luaState);", St::LuaHelper, St::LuaHelper_Push);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void LuaPlugin::GeneratePushGlobalToLua(Class &cls) {
    auto method = ClassMethod{"PushGlobalToLua", Type{"void"}};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("name", Type{"char", PassByType::Pointer, Constness::Const});
    method.mBody.Add("PushToLua(luaState);");
    method.mBody.Add("lua_setglobal(luaState, name);");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}