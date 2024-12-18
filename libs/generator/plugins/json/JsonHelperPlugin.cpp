#include "JsonHelperPlugin.h"
#include "core/St.h"
#include "generator/TypeInfo.h"

namespace holgen {
void JsonHelperPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }
  auto cls = Class{St::JsonHelper, mSettings.mNamespace};
  cls.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
  GenerateParseFunctions(cls);
  GenerateDumpFunctions(cls);
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void JsonHelperPlugin::GenerateParseFunctions(Class &cls) {
  GenerateParseFromFile(cls);
  GenerateBaseParse(cls);
  GenerateParseSingleElem(cls);
  GenerateParseTuple(cls, 2, "std::pair");
  GenerateParseJsonForSmartPointer(cls, "std::shared_ptr", "std::make_shared");
  GenerateParseJsonForSmartPointer(cls, "std::unique_ptr", "std::make_unique");
  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppLists) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppSets) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    for (int withKeyConverter = 0; withKeyConverter < 2; ++withKeyConverter) {
      for (int withElemConverter = 0; withElemConverter < 2; ++withElemConverter) {
        GenerateParseJsonForKeyedContainer(cls, container, withKeyConverter, withElemConverter);
      }
    }
  }
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    GenerateParseLuaObject(cls);
    GenerateParseLuaRegistryObject(cls);
  }
}

void JsonHelperPlugin::GenerateParseFromFile(Class &cls) {
  cls.mHeaderIncludes.AddLocalHeader("FilesystemHelper.h");
  auto method = ClassMethod{St::JsonHelper_ParseFromFile, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  method.mArguments.emplace_back("out", Type{"T", PassByType::Reference});
  method.mArguments.emplace_back(
      "path", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("auto contents = {}::{}(path);", St::FilesystemHelper,
                   St::FilesystemHelper_ReadFile);
  method.mBody.Add("rapidjson::Document doc;");
  method.mBody.Add("doc.Parse(contents.c_str());");
  method.mBody.Add("return {}(out, doc, converter{});", St::JsonHelper_Parse, mLuaStateArgument);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseJsonForSmartPointer(Class &cls, const std::string &pointerType,
                                                        const std::string &pointerGenerator) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");

  {
    auto &out = method.mArguments.emplace_back(
        "out", Type{std::format("{}", pointerType), PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("T");
  }
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("out = {}<T>();", pointerGenerator);
  method.mBody.Add("return Parse(*out.get(), json, converter{});", mLuaStateArgument);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseJsonForSingleElemContainer(Class &cls,
                                                               const std::string &container,
                                                               bool withConverter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  if (withConverter) {
    method.mName = St::JsonHelper_ParseConvertElem;
    method.mTemplateParameters.emplace_back("typename", "SourceType");
  }
  method.mTemplateParameters.emplace_back("typename", "T");

  bool fixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);

  if (fixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");

  {
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("T");
    if (fixedSize)
      out.mType.mTemplateParameters.emplace_back("C");
  }

  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (withConverter) {
    method.mArguments.emplace_back("elemConverter",
                                   Type{"ElemConverter", PassByType::Reference, Constness::Const});
    method.mTemplateParameters.emplace_back("typename", "ElemConverter");
  }
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing {}");)R",
      container);
  if (fixedSize)
    method.mBody.Add("size_t writtenItemCount = 0;");

  CodeBlock modificationBlock;
  std::string elemString = withConverter ? "elemConverter(elem)" : "elem";
  if (TypeInfo::Get().CppSets.contains(container)) {
    modificationBlock.Add("out.insert(std::move({}));", elemString);
  } else if (fixedSize) {
    if (withConverter) {
      modificationBlock.Add("out[writtenItemCount] = std::move({});", elemString);
      // otherwise the element will be parsed in-place
    }
    modificationBlock.Add("++writtenItemCount;");
  } else {
    modificationBlock.Add("out.push_back(std::move({}));", elemString);
  }

  method.mBody.Line() << "for (const auto& data: json.GetArray()) {";
  method.mBody.Indent(1);
  if (fixedSize)
    method.mBody.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(writtenItemCount >= C, false, "Received more data than what the container can handle in {}");)R",
        container);
  if (withConverter) {
    method.mBody.Add("if constexpr (std::same_as<SourceType, rapidjson::Value>) {{");
    method.mBody.Indent(1);
    method.mBody.Add("auto& elem = data;");
    method.mBody.Add(modificationBlock);

    method.mBody.Indent(-1);
    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);
  }
  if (withConverter)
    method.mBody.Add("SourceType elem;");
  else if (!fixedSize)
    method.mBody.Add("T elem;");
  if (withConverter || !fixedSize)
    method.mBody.Add("auto res = {}(elem, data, converter{});", St::JsonHelper_Parse,
                     mLuaStateArgument);
  else
    method.mBody.Add("auto res = {}(out[writtenItemCount], data, converter{});",
                     St::JsonHelper_Parse, mLuaStateArgument);

  method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of {}");)R",
                   container);
  method.mBody.Add(modificationBlock);

  if (withConverter) {
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  }
  method.mBody.Indent(-1);
  method.mBody.Line() << "}"; // range based for on json.GetArray()
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateBaseParse(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  method.mArguments.emplace_back("out", Type{"T", PassByType::Reference});
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("return out.{}(json, converter{});", St::ParseJson, mLuaStateArgument);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseSingleElem(Class &cls, const std::string &type,
                                               const std::string &validator,
                                               const std::string &getter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mIsTemplateSpecialization = true;
  method.mArguments.emplace_back("out", Type{type, PassByType::Reference});
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.{}(), false, "Found type mismatch in json when parsing {}");)R",
      validator, type);
  method.mBody.Line() << "out = json." << getter << "();";
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseTuple(Class &cls, size_t size,
                                          const std::string &tupleClassName) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  auto &tupleArg = method.mArguments.emplace_back("out", Type{tupleClassName});
  tupleArg.mType.mType = PassByType::Reference;
  for (size_t i = 0; i < size; ++i) {
    auto templateParameter = std::format("T{}", i);
    tupleArg.mType.mTemplateParameters.emplace_back(templateParameter);
    method.mTemplateParameters.emplace_back("typename", templateParameter);
  }
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing {}");)R",
      tupleClassName);
  method.mBody.Add("bool res;");
  method.mBody.Add("auto it = json.Begin();");
  for (size_t i = 0; i < size; ++i) {
    method.mBody.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing {}!");)R",
        tupleClassName);
    method.mBody.Add("res = {}(std::get<{}>(out), *it, converter{});", St::JsonHelper_Parse, i,
                     mLuaStateArgument);
    method.mBody.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Parsing {} failed!");)R",
                     tupleClassName);
    method.mBody.Add("++it;");
  }
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing {}!");)R",
      tupleClassName);
  method.mBody.Add("return true;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseLuaObject(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_ParseLuaObject, Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("switch (json.GetType()) {{");

  method.mBody.Add("case rapidjson::kNullType:");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushnil(luaState);");
  method.mBody.Add("break;");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kFalseType:");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushboolean(luaState, false);");
  method.mBody.Add("break;");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kTrueType:");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushboolean(luaState, true);");
  method.mBody.Add("break;");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kObjectType:");
  method.mBody.Indent(1);
  method.mBody.Add("{{");
  method.mBody.Indent(1);
  method.mBody.Add("lua_newtable(luaState);");

  method.mBody.Add("for (const auto& data: json.GetObject()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("{}(data.name, luaState);", St::JsonHelper_ParseLuaObject);
  method.mBody.Add("{}(data.value, luaState);", St::JsonHelper_ParseLuaObject);
  method.mBody.Add("lua_settable(luaState, -3);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Indent(-1);
  method.mBody.Add("break;");
  method.mBody.Add("}}");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kArrayType:");
  method.mBody.Indent(1);
  method.mBody.Add("{{");
  method.mBody.Indent(1);
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("size_t i = 0;");

  method.mBody.Add("for (const auto& data: json.GetArray()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("{}(data, luaState);", St::JsonHelper_ParseLuaObject);
  method.mBody.Add("lua_rawseti(luaState, -2, i);");
  method.mBody.Add("++i;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Indent(-1);
  method.mBody.Add("break;");
  method.mBody.Add("}}");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kStringType:");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushstring(luaState, json.GetString());");
  method.mBody.Add("break;");
  method.mBody.Indent(-1);

  method.mBody.Add("case rapidjson::kNumberType:");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushnumber(luaState, json.GetDouble());");
  method.mBody.Add("break;");
  method.mBody.Indent(-1);

  method.mBody.Add("}}");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseLuaRegistryObject(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_ParseLuaRegistryObject, Type{"int"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("{}(json, luaState);", St::JsonHelper_ParseLuaObject);
  method.mBody.Add("return luaL_ref(luaState, LUA_REGISTRYINDEX);");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpToFile(Class &cls) {
  cls.mHeaderIncludes.AddStandardHeader("filesystem");
  cls.mSourceIncludes.AddLibHeader("rapidjson/stringbuffer.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/writer.h");
  cls.mSourceIncludes.AddStandardHeader("fstream");
  auto method = ClassMethod{St::JsonHelper_DumpToFile, Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back(
      "path", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mBody.Add("rapidjson::StringBuffer sb;");
  method.mBody.Add("rapidjson::Writer<rapidjson::StringBuffer> writer(sb);");
  method.mBody.Add("json.Accept(writer);");
  method.mBody.Add("std::ofstream out(path, std::ios_base::binary);");
  method.mBody.Add("out.write(sb.GetString(), sb.GetSize());");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpFunctions(Class &cls) {
  GenerateDumpToFile(cls);
  GenerateBaseDump(cls);
  GenerateDumpSingleElem(cls);
  for (auto &container: TypeInfo::Get().CppSingleElemContainers) {
    GenerateDumpForSingleElemContainer(cls, container);
  }
  for (auto &container: TypeInfo::Get().CppKeyedContainers) {
    GenerateDumpForKeyedContainer(cls, container);
  }
  GenerateDumpTuple(cls, 2, "std::pair");

  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    cls.mHeaderIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddStandardHeader("cstring");
    GenerateDumpLuaObject(cls);
    GenerateDumpLuaRegistryObject(cls);
  }
}

void JsonHelperPlugin::GenerateDumpLuaObject(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_DumpLuaObject, Type{"rapidjson::Value"},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("idx", Type{"int"});
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("stringify", Type{"bool"});

  method.mBody.Add("auto luaType = lua_type(luaState, idx);");
  method.mBody.Add("switch (luaType) {{");

  method.mBody.Add("case LUA_TNIL:");
  method.mBody.Indent(1);
  method.mBody.Add("return rapidjson::Value(rapidjson::kNullType);");
  method.mBody.Indent(-1);

  method.mBody.Add("case LUA_TBOOLEAN:");
  method.mBody.Indent(1);
  method.mBody.Add("return rapidjson::Value(lua_toboolean(luaState, idx));");
  method.mBody.Indent(-1);

  method.mBody.Add("case LUA_TNUMBER:");
  method.mBody.Indent(1);
  method.mBody.Add("if (stringify) {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto val = std::to_string(lua_tonumber(luaState, idx));");
  method.mBody.Add("return rapidjson::Value(val.c_str(), val.size(), doc.GetAllocator());");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("return rapidjson::Value(lua_tonumber(luaState, idx));");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);

  method.mBody.Add("case LUA_TSTRING:");
  method.mBody.Indent(1);
  method.mBody.Add("{{");
  method.mBody.Indent(1);
  method.mBody.Add("auto val = lua_tostring(luaState, idx);");
  method.mBody.Add("return rapidjson::Value(val, std::strlen(val), doc.GetAllocator());");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);

  method.mBody.Add("case LUA_TTABLE:");
  method.mBody.Indent(1);
  method.mBody.Add("{{");
  method.mBody.Indent(1);
  method.mBody.Add("auto val = rapidjson::Value(rapidjson::kObjectType);");
  method.mBody.Add("lua_pushvalue(luaState, idx);");
  method.mBody.Add("lua_pushnil(luaState);");

  method.mBody.Add("while (lua_next(luaState, -2)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto value = {}(-1, doc, luaState, false);", St::JsonHelper_DumpLuaObject);
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("auto key = {}(-1, doc, luaState, true);", St::JsonHelper_DumpLuaObject);
  method.mBody.Add("val.AddMember(key, value, doc.GetAllocator());");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return val;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);

  method.mBody.Add("default:");
  method.mBody.Indent(1);
  method.mBody.Add("HOLGEN_FAIL(\"Unexpected type when serializing lua data: {{}}\", luaType);");
  method.mBody.Indent(-1);

  method.mBody.Add("}}");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpLuaRegistryObject(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_DumpLuaRegistryObject, Type{"rapidjson::Value"},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("data", Type{"int"});
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("lua_rawgeti(luaState, LUA_REGISTRYINDEX, data);");
  method.mBody.Add("auto res = {}(-1, doc, luaState, false);", St::JsonHelper_DumpLuaObject);
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return res;");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpSingleElem(Class &cls) {
  for (auto &type: {"int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t",
                    "uint64_t", "float", "double", "bool"}) {
    auto method = GenerateDumpMethod(type);
    method.mBody.Add("return rapidjson::Value(data);");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
  {
    auto method = GenerateDumpMethod("std::string");
    method.mBody.Add("return rapidjson::Value(data.c_str(), data.size(), doc.GetAllocator());");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void JsonHelperPlugin::GenerateBaseDump(Class &cls) {
  auto method = GenerateDumpMethod("T");
  method.mTemplateParameters.emplace_back("typename", "T");
  method.mBody.Add("return data.{}(doc{});", St::DumpJson, mLuaStateArgument);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

ClassMethod JsonHelperPlugin::GenerateDumpMethod(const std::string &type) {
  auto method = ClassMethod{St::JsonHelper_Dump, Type{"rapidjson::Value"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("data", Type{type, PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  return method;
}

void JsonHelperPlugin::GenerateDumpForSingleElemContainer(Class &cls,
                                                          const std::string &container) {
  auto method = GenerateDumpMethod(container);
  method.mTemplateParameters.emplace_back("typename", "T");
  method.mArguments.front().mType.mTemplateParameters.emplace_back("T");
  if (container == "std::array") {
    method.mTemplateParameters.emplace_back("size_t", "Size");
    method.mArguments.front().mType.mTemplateParameters.emplace_back("Size");
  }
  method.mBody.Add("auto val = rapidjson::Value(rapidjson::kArrayType);");
  method.mBody.Add("val.Reserve(data.size(), doc.GetAllocator());");
  method.mBody.Add("for (auto& elem: data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("val.PushBack({}(elem, doc{}), doc.GetAllocator());", St::JsonHelper_Dump,
                   mLuaStateArgument);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Add("return val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpForKeyedContainer(Class &cls, const std::string &container) {

  auto method = GenerateDumpMethod(container);
  method.mTemplateParameters.emplace_back("typename", "K");
  method.mTemplateParameters.emplace_back("typename", "V");
  method.mArguments.front().mType.mTemplateParameters.emplace_back("K");
  method.mArguments.front().mType.mTemplateParameters.emplace_back("V");
  method.mBody.Add("rapidjson::Type valueType;");
  method.mBody.Add("if constexpr ({} || {}<K>) {{", GetIsStringTypeCondition("K"), St::EnumConcept);
  method.mBody.Indent(1);
  method.mBody.Add("valueType = rapidjson::kObjectType;");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{", St::JsonHelper_Dump);
  method.mBody.Indent(1);
  method.mBody.Add("valueType = rapidjson::kArrayType;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}", St::JsonHelper_Dump);
  method.mBody.Add("auto val = rapidjson::Value(valueType);");

  method.mBody.Add("for (auto &[k, v]: data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if constexpr ({} || {}<K>) {{", GetIsStringTypeCondition("K"), St::EnumConcept);
  method.mBody.Indent(1);
  method.mBody.Add("val.AddMember({0}(k, doc{1}), {0}(v, doc{1}), doc.GetAllocator());",
                   St::JsonHelper_Dump, mLuaStateArgument);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{", St::JsonHelper_Dump);
  method.mBody.Indent(1);
  method.mBody.Add("auto elem = rapidjson::Value(rapidjson::kArrayType);");
  method.mBody.Add("elem.Reserve(2, doc.GetAllocator());");
  method.mBody.Add("elem.PushBack({}(k, doc{}), doc.GetAllocator());", St::JsonHelper_Dump,
                   mLuaStateArgument);
  method.mBody.Add("elem.PushBack({}(v, doc{}), doc.GetAllocator());", St::JsonHelper_Dump,
                   mLuaStateArgument);
  method.mBody.Add("val.PushBack(elem, doc.GetAllocator());", St::JsonHelper_Dump);
  method.mBody.Indent(-1);
  method.mBody.Add("}}", St::JsonHelper_Dump);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("return val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateDumpTuple(Class &cls, size_t size, const std::string &container) {
  auto method = GenerateDumpMethod(container);
  for (size_t i = 0; i < size; ++i) {
    auto tName = std::format("T{}", i);
    method.mTemplateParameters.emplace_back("typename", tName);
    method.mArguments.front().mType.mTemplateParameters.emplace_back(tName);
  }
  method.mBody.Add("auto val = rapidjson::Value(rapidjson::kArrayType);");
  method.mBody.Add("val.Reserve({}, doc.GetAllocator());", size);

  for (size_t i = 0; i < size; ++i) {
    method.mBody.Add("val.PushBack({}(std::get<{}>(data), doc{}), doc.GetAllocator());",
                     St::JsonHelper_Dump, i, mLuaStateArgument);
  }

  method.mBody.Add("return val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseSingleElem(Class &cls) {
  GenerateParseSingleElem(cls, "int8_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int16_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int32_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int64_t", "IsInt64", "GetInt64");
  GenerateParseSingleElem(cls, "uint8_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint16_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint32_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint64_t", "IsUint64", "GetUint64");
  GenerateParseSingleElem(cls, "float", "IsNumber", "GetFloat");
  GenerateParseSingleElem(cls, "double", "IsNumber", "GetDouble");
  GenerateParseSingleElem(cls, "bool", "IsBool", "GetBool");
  GenerateParseSingleElem(cls, "std::string", "IsString", "GetString");
}

void JsonHelperPlugin::GenerateParseJsonForKeyedContainerElem(
    CodeBlock &codeBlock, const std::string &container, bool withKeyConverter,
    bool withElemConverter, const std::string &keyVariable, const std::string &valueVariable) {
  if (withKeyConverter) {
    codeBlock.Line() << "KeySourceType keyInJson;";
    codeBlock.Add("auto res = {}(keyInJson, {}, converter{});", St::JsonHelper_Parse, keyVariable,
                  mLuaStateArgument);
    codeBlock.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of {}");)R", container);
    codeBlock.Line() << "K key = std::move(keyConverter(keyInJson));";
  } else {
    codeBlock.Line() << "K key;";
    codeBlock.Add("auto res = {}(key, {}, converter{});", St::JsonHelper_Parse, keyVariable,
                  mLuaStateArgument);
    codeBlock.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of {}");)R", container);
  }

  codeBlock.Line() << "auto[it, insertRes] = out.try_emplace(key, V());";
  codeBlock.Add(
      "if constexpr (std::is_integral_v<K> || std::is_same_v<K, std::string> || {}<K>) {{",
      St::EnumConcept);
  codeBlock.Indent(1);
  codeBlock.Add(
      R"R(HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {{}} when parsing {}", key);)R",
      container);
  codeBlock.Indent(-1);
  codeBlock.Add("}} else {{");
  codeBlock.Indent(1);
  codeBlock.Add(
      R"R(HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key when parsing {}");)R",
      container);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  if (withElemConverter) {
    codeBlock.Add("ElemSourceType valueRaw;");
    codeBlock.Add("res = {}(valueRaw, {}, converter{});", St::JsonHelper_Parse, valueVariable,
                  mLuaStateArgument);
    codeBlock.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of {}");)R",
                  container);
    codeBlock.Add("it->second = std::move(elemConverter(valueRaw));");
  } else {
    codeBlock.Add("res = {}(it->second, {}, converter{});", St::JsonHelper_Parse, valueVariable,
                  mLuaStateArgument);
    codeBlock.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of {}");)R",
                  container);
  }
}

void JsonHelperPlugin::GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container,
                                                          bool withKeyConverter,
                                                          bool withElemConverter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};

  if (withKeyConverter) {
    method.mName = St::JsonHelper_ParseConvertKey;
    method.mTemplateParameters.emplace_back("typename", "KeySourceType");
  }
  if (withElemConverter) {
    method.mName = St::JsonHelper_ParseConvertElem;
    method.mTemplateParameters.emplace_back("typename", "ElemSourceType");
  }


  method.mTemplateParameters.emplace_back("typename", "K");
  method.mTemplateParameters.emplace_back("typename", "V");

  {
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("K");
    out.mType.mTemplateParameters.emplace_back("V");
  }
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});

  if (withKeyConverter) {
    method.mTemplateParameters.emplace_back("typename", "KeyConverter");
    method.mArguments.emplace_back("keyConverter",
                                   Type{"KeyConverter", PassByType::Reference, Constness::Const});
  }
  if (withElemConverter) {
    method.mTemplateParameters.emplace_back("typename", "ElemConverter");
    method.mArguments.emplace_back("elemConverter",
                                   Type{"ElemConverter", PassByType::Reference, Constness::Const});
  }
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  if (withKeyConverter && withElemConverter) {
    method.mName = St::JsonHelper_ParseConvertKeyElem;
  }

  if (withKeyConverter)
    method.mBody.Add("if constexpr ({} || {}<KeySourceType>) {{",
                     GetIsStringTypeCondition("KeySourceType"), St::EnumConcept);
  else
    method.mBody.Add("if constexpr ({} || {}<K>) {{", GetIsStringTypeCondition("K"),
                     St::EnumConcept);
  method.mBody.Indent(1);

  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing {}");)R",
      container);

  method.mBody.Line() << "for (const auto& data: json.GetObject()) {";
  method.mBody.Indent(1);

  GenerateParseJsonForKeyedContainerElem(method.mBody, container, withKeyConverter,
                                         withElemConverter, "data.name", "data.value");

  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing {}");)R",
      container);
  method.mBody.Add("for (const auto& data: json.GetArray()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto& elemKey = data[0];");
  method.mBody.Add("auto& elemValue = data[1];");
  GenerateParseJsonForKeyedContainerElem(method.mBody, container, withKeyConverter,
                                         withElemConverter, "elemKey", "elemValue");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
