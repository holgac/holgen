#include "LuaIndexMetaMethodPlugin.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

void LuaIndexMetaMethodPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua) ||
        cls.mStruct->GetMatchingAttribute(Annotations::LuaFuncTable,
                                          Annotations::LuaFuncTable_Publisher))
      continue;
    Process(cls);
  }
}

void LuaIndexMetaMethodPlugin::Process(Class &cls) {
  auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(R"R(HOLGEN_WARN("Unexpected lua field: {}.{{}}", key);)R",
                             cls.mStruct->mName);
  stringSwitcherElseCase.Add("return 0;");
  StringSwitcher switcher("key", std::move(stringSwitcherElseCase));

  GenerateIndexMetaMethodForFields(cls, switcher);
  GenerateIndexMetaMethodForExposedMethods(cls, switcher);
  if (cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    GenerateIndexMetaMethodForFuncTable(cls, switcher);
  }

  if (!switcher.IsEmpty()) {
    method.mBody.Add("const char *key = lua_tostring(luaState, -1);");
    method.mBody.Add(std::move(switcher.Generate()));
    method.mBody.Line() << "return 1;";
  } else {
    method.mBody.Line() << "return 0;";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForFuncTable(Class &cls,
                                                                   StringSwitcher &switcher) {
  if (cls.mStruct &&
      cls.mStruct->GetMatchingAnnotation(Annotations::LuaFuncTable,
                                         Annotations::LuaFuncTable_Publisher)) {
    switcher.AddCase(St::LuaTable_TableFieldInIndexMethod, [&](CodeBlock &switchBlock) {
      GenerateInstanceGetter(cls, switchBlock, -2, "instance");
      switchBlock.Add("{}::{}<false>(\"{}\", luaState);", St::LuaHelper, St::LuaHelper_Push,
                      cls.mName);
    });

  } else {
    switcher.AddCase(St::LuaTable_TableFieldInIndexMethod, [&](CodeBlock &switchBlock) {
      GenerateInstanceGetter(cls, switchBlock, -2, "instance");
      switchBlock.Add("{}::{}<false>(instance->{}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                      Naming().FieldNameInCpp(St::LuaTable_TableField));
    });
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForFields(Class &cls,
                                                                StringSwitcher &switcher) {
  for (auto &field: cls.mFields) {
    // TODO: handle variant type (should call setter instead of direct assignment)
    if (!field.mField || field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Lua) ||
        field.mField->mType.mName == St::UserData)
      continue;
    auto fieldClass = mProject.GetClass(field.mType.mName);
    if (fieldClass && fieldClass->mStruct &&
        (fieldClass->mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
         fieldClass->mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Lua)))
      continue;
    bool isRef = field.mField->mType.mName == "Ref";
    switcher.AddCase(Naming().FieldNameInLua(*field.mField), [&](CodeBlock &switchBlock) {
      GenerateIndexForField(cls, field, switchBlock);
    });
    if (isRef && field.mType.mType != PassByType::Pointer) {
      auto underlyingStruct =
          mProject.mProject.GetStruct(field.mField->mType.mTemplateParameters.front().mName);
      if (underlyingStruct->GetAnnotation(Annotations::Managed)) {
        switcher.AddCase(Naming().FieldNameInLua(*field.mField, true), [&](CodeBlock &switchBlock) {
          GenerateInstanceGetter(cls, switchBlock, -2, "instance");
          switchBlock.Add("{}::{}<false>({}::{}(instance->{}), luaState);", St::LuaHelper,
                          St::LuaHelper_Push, field.mField->mType.mTemplateParameters[0].mName,
                          St::ManagedObject_Getter, field.mName);
        });
      }
    }
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexMetaMethodForExposedMethods(Class &cls,
                                                                        StringSwitcher &switcher) {
  for (auto &exposedMethod: cls.mMethods) {
    if (!ShouldGenerateWrapper(exposedMethod))
      continue;
    GenerateMethodCaller(cls, exposedMethod, Naming().LuaMethodCaller(exposedMethod));
    std::string methodSuffix;
    if (exposedMethod.mFunction &&
        exposedMethod.mFunction->GetMatchingAttribute(Annotations::Func,
                                                      Annotations::Func_OverloadSuffix)) {
      methodSuffix = exposedMethod.mFunction
                         ->GetMatchingAttribute(Annotations::Func, Annotations::Func_OverloadSuffix)
                         ->mValue.mName;
    }
    switcher.AddCase(exposedMethod.mName + methodSuffix, [&](CodeBlock &switchBlock) {
      // TODO: LuaHelper::Push should work with functions
      switchBlock.Add("lua_pushcfunction(luaState, {}::{});", cls.mName,
                      Naming().LuaMethodCaller(exposedMethod));
    });
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexForField(Class &cls, ClassField &field,
                                                     CodeBlock &switchBlock) {
  GenerateInstanceGetter(cls, switchBlock, -2, "instance");
  switchBlock.Add("HOLGEN_WARN_AND_RETURN_IF(!instance, 0, \"Requesting for {}.{} with "
                  "an invalid lua proxy object!\");",
                  cls.mName, field.mField ? field.mField->mName : field.mName);
  if (field.mField->mType.mName == St::Variant) {
    GenerateIndexForVariantField(cls, field, switchBlock);
  } else if (field.mField->mType.mName == St::Lua_CustomData) {
    GenerateIndexForRegistryData(field, switchBlock);
  } else {
    switchBlock.Add("{}::{}<false>(instance->{}, luaState);", St::LuaHelper, St::LuaHelper_Push,
                    field.mName);
  }
}

void LuaIndexMetaMethodPlugin::GenerateIndexForVariantField(Class &cls, ClassField &field,
                                                            CodeBlock &switchBlock) {
  auto enumField =
      cls.GetField(Naming().FieldNameInCpp(field.mField->GetAnnotation(Annotations::Variant)
                                               ->GetAttribute(Annotations::Variant_TypeField)
                                               ->mValue.mName));
  switchBlock.Add("switch (instance->{}.GetValue()) {{", enumField->mName);
  for (auto &otherCls: mProject.mClasses) {
    if (!otherCls.mStruct || !otherCls.mStruct->GetAnnotation(Annotations::Variant))
      continue;
    auto variantAnnotation = otherCls.mStruct->GetAnnotation(Annotations::Variant);
    if (variantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName !=
        enumField->mType.mName)
      continue;
    switchBlock.Add("case {}::{}:", enumField->mType.ToString(true),
                    variantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
    switchBlock.Indent(1);
    switchBlock.Add("{}::{}<false>(instance->{}(), luaState);", St::LuaHelper, St::LuaHelper_Push,
                    Naming().VariantGetterNameInCpp(*field.mField, *otherCls.mStruct));
    switchBlock.Add("break;");
    switchBlock.Indent(-1);
  }
  switchBlock.Add("default:");
  switchBlock.Indent(1);
  switchBlock.Add("lua_pushnil(luaState);");
  switchBlock.Indent(-1);
  switchBlock.Add("}}");
}

void LuaIndexMetaMethodPlugin::GenerateIndexForRegistryData(ClassField &field,
                                                            CodeBlock &switchBlock) {
  switchBlock.Add("lua_rawgeti(luaState, LUA_REGISTRYINDEX, instance->{});", field.mName);
}

bool LuaIndexMetaMethodPlugin::ShouldGenerateWrapper(const ClassMethod &method) const {
  if (!method.mExposeToLua)
    return false;
  if (method.mFunction &&
      (method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_OnDataLoad) ||
       method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Cpp)))
    return false;
  return true;
}

} // namespace holgen
