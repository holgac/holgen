#include "JsonDumpPlugin.h"

#include "core/St.h"

namespace holgen {
void JsonDumpPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }

  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void JsonDumpPlugin::ProcessStruct(Class &cls) {
  auto method =
      ClassMethod{St::DumpJson, Type{"rapidjson::Value"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    cls.mHeaderIncludes.AddLibHeader("lua.hpp");
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  }
  method.mBody.Add("rapidjson::Value val(rapidjson::kObjectType);");
  ProcessStructFields(cls, method.mBody);
  ProcessStructVariantFields(cls, method.mBody);

  method.mBody.Add("return val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonDumpPlugin::ProcessEnum(Class &cls) {
  auto method =
      ClassMethod{St::DumpJson, Type{"rapidjson::Value"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    cls.mHeaderIncludes.AddLibHeader("lua.hpp");
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  }
  method.mBody.Add("return rapidjson::Value(GetValue());");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonDumpPlugin::GenerateForField(CodeBlock &codeBlock, const ClassField &field,
                                      const std::string &fieldName) {
  std::string constructor;
  if (field.mField && field.mField->mType.mName == St::Lua_CustomData &&
      mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    constructor = std::format("{}::{}({}, doc, luaState)", St::JsonHelper,
                              St::JsonHelper_DumpLuaRegistryObject, field.mName);
  else
    constructor = std::format("{}::{}({}, doc{})", St::JsonHelper, St::JsonHelper_Dump, field.mName,
                              mLuaStateArgument);
  codeBlock.Add("val.AddMember(\"{}\", {}, doc.GetAllocator());", fieldName, constructor);
}

void JsonDumpPlugin::ProcessStructFields(Class &cls, CodeBlock &codeBlock) {
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::LuaFuncTable))
    GenerateForField(codeBlock, *cls.GetField(Naming().FieldNameInCpp(St::LuaTable_TableField)),
                     St::LuaTable_TableField);
  for (auto &field: cls.mFields) {
    if (IsContainerOfDataManager(cls, field))
      continue;
    const std::string *variantRawName = nullptr;
    bool isVariantTypeField = field.IsVariantTypeField(cls, &variantRawName, Naming());
    if (!ShouldProcess(field, isVariantTypeField))
      continue;
    if (field.mField && field.mField->GetAnnotation(Annotations::Variant))
      continue;

    GenerateForField(codeBlock, field, field.mField ? field.mField->mName : *variantRawName);
  }
}

void JsonDumpPlugin::ProcessStructVariantFields(Class &cls, CodeBlock &codeBlock) {
  auto variantData = cls.GetVariantData();
  for (auto &[variantTypeFieldName, variantFields]: variantData) {
    auto variantEnumName = cls.GetField(Naming().FieldNameInCpp(variantTypeFieldName))->mType.mName;
    auto variantClasses = mProject.GetVariantClassesOfEnum(variantEnumName);
    codeBlock.Add("switch ({}.GetValue()) {{", Naming().FieldNameInCpp(variantTypeFieldName));
    for (auto &[variantClass, enumEntry]: variantClasses) {
      codeBlock.Add("case {}::{}:", variantEnumName, enumEntry->mName);
      codeBlock.Indent(1);
      for (auto &variantField: variantFields) {
        codeBlock.Add(
            "val.AddMember(\"{}\", {}()->{}(doc{}), doc.GetAllocator());",
            variantField->mField->mName,
            Naming().VariantGetterNameInCpp(*variantField->mField, *variantClass->mStruct),
            St::DumpJson, mLuaStateArgument);
      }
      codeBlock.Add("break;");
      codeBlock.Indent(-1);
    }
    codeBlock.Add("}}");
  }
}
} // namespace holgen
