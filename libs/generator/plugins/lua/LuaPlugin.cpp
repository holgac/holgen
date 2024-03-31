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
        EnrichClass(cls, *cls.mStruct);
      else if (cls.mEnum)
        EnrichClass(cls, *cls.mEnum);
    }
  }

  void LuaPlugin::CreatePushIndexMetaMethod(Class &cls) {
    auto &method = cls.mMethods.emplace_back(
        "PushIndexMetaMethod", Type{"void"}, Visibility::Private, Constness::NotConst, Staticness::Static);
    auto &codeBlock = method.mBody;
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    auto structDefinition = cls.mStruct;
    if (structDefinition == nullptr)
      return;
    codeBlock.Add("lua_pushstring(luaState, \"__index\");");
    codeBlock.Add("lua_pushcfunction(luaState, [](lua_State* ls) {{");
    codeBlock.Indent(1);
    codeBlock.Add("auto instance = {}::ReadFromLua(ls, -2);", cls.mName);
    codeBlock.Add("const char* key = lua_tostring(ls, -1);");
    bool isFirst = true;
    for (auto &fieldDefinition: structDefinition->mFields) {
      if (fieldDefinition.GetAnnotation(Annotations::NoLua))
        continue;
      auto &generatedField = *cls.GetField(Naming().FieldNameInCpp(fieldDefinition));
      bool isRef = fieldDefinition.mType.mName == "Ref";
      if (isFirst) {
        codeBlock.Add("if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(fieldDefinition));
        isFirst = false;
      } else {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(fieldDefinition));
      }
      codeBlock.Indent(1);
      codeBlock.Add("{}::{}(instance->{}, ls);", St::LuaHelper, St::LuaHelper_Push,
                    Naming().FieldNameInCpp(fieldDefinition));
      codeBlock.Indent(-1);
      if (isRef && generatedField.mType.mType != PassByType::Pointer) {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{",
                      Naming().FieldNameInLua(fieldDefinition, true));
        codeBlock.Indent(1);
        codeBlock.Add("{}::{}({}::{}(instance->{}), ls);", St::LuaHelper, St::LuaHelper_Push,
                      fieldDefinition.mType.mTemplateParameters[0].mName, St::ManagedObject_Getter,
                      Naming().FieldNameInCpp(fieldDefinition));
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
      for (auto& arg: luaMethod.mArguments) {
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
  }

  void LuaPlugin::CreatePushNewIndexMetaMethod(Class &cls) {
    auto &method = cls.mMethods.emplace_back(
        "PushNewIndexMetaMethod", Type{"void"}, Visibility::Private, Constness::NotConst, Staticness::Static);
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
    for (auto &fieldDefinition: structDefinition->mFields) {
      // TODO: This can be a bit more nuanced, maybe allow getting but not setting?
      if (fieldDefinition.GetAnnotation(Annotations::NoLua))
        continue;
      if (isFirst) {
        codeBlock.Add("if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(fieldDefinition));
        isFirst = false;
      } else {
        codeBlock.Add("}} else if (0 == strcmp(\"{}\", key)) {{", Naming().FieldNameInLua(fieldDefinition));
      }
      // TODO: Implement else case
      // TODO: Make this work with pointers
      // TODO: Make this work with nested structs
      codeBlock.Indent(1);
      // TODO: This appends to containers, so a=[1] a=[2] results in a=[1,2].
      codeBlock.Add("{}::{}(instance->{}, ls, -1);", St::LuaHelper, St::LuaHelper_Read,
                    Naming().FieldNameInCpp(fieldDefinition));
      codeBlock.Indent(-1);
    }
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __newindex
    codeBlock.Add("lua_settable(luaState, -3);");
  }

  void LuaPlugin::AddReadFromLua(Class &cls, const StructDefinition &structDefinition) {
    // Only works with negative indices
    // TODO: generate comments?
    auto &method = cls.mMethods.emplace_back(
        "ReadFromLua", Type{structDefinition.mName, PassByType::Pointer},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    if (structDefinition.GetAnnotation(Annotations::Managed)) {
      method.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
      method.mBody.Add("lua_gettable(luaState, idx - 1);");
      auto idField = cls.GetField(Naming().FieldNameInCpp(*structDefinition.GetIdField()));
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
  }

  void LuaPlugin::AddPushToLua(Class &cls, const StructDefinition &structDefinition) {
    auto &pushToLua = cls.mMethods.emplace_back("PushToLua", Type{"void"}, Visibility::Public, Constness::Const);
    pushToLua.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

    pushToLua.mBody.Add("lua_newtable(luaState);");
    if (structDefinition.GetAnnotation(Annotations::Managed)) {
      auto idField = cls.GetField(Naming().FieldNameInCpp(*structDefinition.GetIdField()));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      pushToLua.mBody.Add("{} id = {};", tempType, idField->mName);
      pushToLua.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
      pushToLua.mBody.Add("lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));");
    } else {
      // TODO: debug generator? Or generate extra debug stuff that's gated behind a macro?
      // Specifying object type would help here
      pushToLua.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
      pushToLua.mBody.Add("lua_pushlightuserdata(luaState, (void*)this);");
    }
    pushToLua.mBody.Add("lua_settable(luaState, -3);");
    // Do this last so that metamethods don't get called during object construction
    // TODO: Use Naming for metatable naming
    pushToLua.mBody.Add("lua_getglobal(luaState, \"{}Meta\");", cls.mName);
    pushToLua.mBody.Add("lua_setmetatable(luaState, -2);");
  }

  void LuaPlugin::EnrichClass(Class &cls, const StructDefinition &structDefinition) {
    if (structDefinition.GetAnnotation(Annotations::NoLua))
      return;
    cls.mGlobalForwardDeclarations.insert({"struct", "lua_State"});
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");
    AddPushToLua(cls, structDefinition);
    AddPushGlobalToLua(cls, structDefinition);
    AddReadFromLua(cls, structDefinition);

    CreatePushIndexMetaMethod(cls);
    CreatePushNewIndexMetaMethod(cls);

    auto &createLuaMetatable = cls.mMethods.emplace_back(
        "CreateLuaMetatable", Type{"void"}, Visibility::Public, Constness::NotConst, Staticness::Static);
    createLuaMetatable.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    createLuaMetatable.mBody.Add("lua_newtable(luaState);");
    createLuaMetatable.mBody.Add("PushIndexMetaMethod(luaState);");
    createLuaMetatable.mBody.Add("PushNewIndexMetaMethod(luaState);");
    createLuaMetatable.mBody.Add("lua_setglobal(luaState, \"{}Meta\");", cls.mName);
  }

  void LuaPlugin::EnrichClass(Class &cls, const EnumDefinition &enumDefinition) {
    if (enumDefinition.GetAnnotation(Annotations::NoLua))
      return;

    cls.mGlobalForwardDeclarations.insert({"struct", "lua_State"});
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");

    auto &pushToLua = cls.mMethods.emplace_back("PushToLua", Type{"void"}, Visibility::Public, Constness::Const);
    pushToLua.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    pushToLua.mBody.Add("{}::{}(mValue, luaState);", St::LuaHelper, St::LuaHelper_Push);
  }

  void LuaPlugin::AddPushGlobalToLua(Class &cls, const StructDefinition &structDefinition __attribute__((unused))) {
    auto &method = cls.mMethods.emplace_back("PushGlobalToLua", Type{"void"});
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("name", Type{"char", PassByType::Pointer, Constness::Const});
    method.mBody.Add("PushToLua(luaState);");
    method.mBody.Add("lua_setglobal(luaState, name);");
  }

}