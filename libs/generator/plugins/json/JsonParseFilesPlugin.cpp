#include "JsonParseFilesPlugin.h"

#include <vector>
#include "generator/utils/NamingConvention.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/TypeInfo.h"

namespace holgen {
namespace {
std::string ParseJson = "ParseJson";
std::string ParseFiles = "ParseFiles";
} // namespace

void JsonParseFilesPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    if (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Json) ||
        cls.mStruct->GetAnnotation(Annotations::DataManager) == nullptr)
      continue;
    GenerateParseFiles(cls);
  }
}

void JsonParseFilesPlugin::GenerateParseFiles(Class &cls) {
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");

  cls.mHeaderIncludes.AddStandardHeader("filesystem");
  cls.mSourceIncludes.AddStandardHeader("queue");
  cls.mSourceIncludes.AddStandardHeader("vector");
  cls.mSourceIncludes.AddLocalHeader(St::FilesystemHelper + ".h");
  auto method = ClassMethod{ParseFiles, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back(
      "rootPath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("selfName",
                                 Type{"std::string", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converterArg",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua))
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  GenerateConverterPopulators(cls, method);

  GenerateReadSelf(method.mBody);
  GenerateFilesByName(method);

  bool isFirst = true;
  auto processOrder = GetProcessOrder(cls);
  for (auto &fieldPtr: processOrder) {
    auto &field = *fieldPtr;
    auto &fieldDefinition = *field.mField;
    const TypeDefinition *templateParameter;
    if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
      templateParameter = &fieldDefinition.mType.mTemplateParameters.back();
    } else {
      templateParameter = &fieldDefinition.mType.mTemplateParameters.front();
    }

    {
      auto line = method.mBody.Line();
      if (isFirst) {
        line << "auto it";
        isFirst = false;
      } else {
        line << "it";
      }
      line << " = filesByName.find(\"" << fieldDefinition.mName << "\");";
    }
    method.mBody.Add("if (it != filesByName.end()) {{");
    method.mBody.Indent(1);

    method.mBody.Add("for (const auto& filePath: it->second) {{");
    method.mBody.Indent(1);
    method.mBody.Add("auto contents = {}::{}(filePath);", St::FilesystemHelper,
                     St::FilesystemHelper_ReadFile);
    method.mBody.Add("rapidjson::Document doc;");
    method.mBody.Add("doc.Parse(contents.c_str());");
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {{}}: It is supposed to contain a list of {} entries", filePath.string());)",
        field.mType.mTemplateParameters.front().mName);
    method.mBody.Add("for (auto& jsonElem: doc.GetArray()) {{"); // if (!doc.IsArray())
    method.mBody.Indent(1);
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {{}}", filePath.string());)");
    Type type(mProject, fieldDefinition.mDefinitionSource, *templateParameter);
    method.mBody.Add("{}elem;", type.ToString(false)); // if (!doc.IsArray())
    method.mBody.Add("auto res = {}::{}(elem, jsonElem, converter{});", St::JsonHelper,
                     St::JsonHelper_Parse,
                     mLuaStateArgument); // if (!doc.IsArray())
    method.mBody.Add(
        R"(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {{}}", filePath.string());)");
    if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName)) {
      method.mBody.Add("{}(elem);", Naming().ContainerElemAdderNameInCpp(fieldDefinition));
    } else if (!field.mField->GetAnnotation(Annotations::Index)) {
      method.mBody.Add("{}(std::move(elem));",
                       Naming().ContainerElemAdderNameInCpp(fieldDefinition));
    } else {
      method.mBody.Add("auto elemPtr = {}(std::move(elem));",
                       Naming().ContainerElemAdderNameInCpp(fieldDefinition));
      method.mBody.Add("if (elemPtr == nullptr) {{");
      method.mBody.Indent(1);
      bool isFirstIndex = true;
      bool isSecondIndex = true;
      std::stringstream conditions;
      std::string firstIndexField;
      std::string firstIndexFieldGetter;
      for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
        if (isFirstIndex) {
          firstIndexField = annotation.GetAttribute(Annotations::Index_On)->mValue.mName;
          firstIndexFieldGetter = Naming().FieldGetterNameInCpp(firstIndexField);
          method.mBody.Add("auto existingElem = {}(elem.{}());",
                           Naming().ContainerIndexGetterNameInCpp(*field.mField, annotation),
                           firstIndexFieldGetter);
          isFirstIndex = false;
          continue;
        }
        if (isSecondIndex)
          isSecondIndex = false;
        else
          conditions << " || ";
        conditions << std::format(
            "existingElem != {}(elem.{}())",
            Naming().ContainerIndexGetterNameInCpp(*field.mField, annotation),
            Naming().FieldGetterNameInCpp(
                annotation.GetAttribute(Annotations::Index_On)->mValue.mName));
      }
      if (!isSecondIndex) {
        method.mBody.Add("HOLGEN_WARN_AND_CONTINUE_IF({}, \"Invalid {} element ({}={{}}) matching "
                         "multiple indices cannot be parsed!\", elem.{}());",
                         conditions.str(), field.mType.mName, firstIndexField,
                         firstIndexFieldGetter);
      }
      method.mBody.Add("{}::{}(*existingElem, jsonElem, converter{});", St::JsonHelper,
                       St::JsonHelper_Parse,
                       mLuaStateArgument); // if (!doc.IsArray())
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    }
    method.mBody.Indent(-1);
    method.mBody.Add("}}"); // for (jsonElem: doc.GetArray())

    method.mBody.Indent(-1);
    method.mBody.Add("}}"); // for(path: filesByName[field])

    method.mBody.Indent(-1);
    method.mBody.Add("}}"); // if (it != filesByName.end())
  }
  GenerateOnDataLoadCalls(cls, method.mBody);
  method.mBody.Add("return true;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonParseFilesPlugin::GenerateConverterPopulators(Class &cls, ClassMethod &method) {
  CodeBlock codeBlock;
  for (const auto &field: cls.mFields) {
    if (!field.mField || !field.mField->GetAnnotation(Annotations::Container))
      continue;
    for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
      auto &underlyingClass =
          *mProject.GetClass(field.mField->mType.mTemplateParameters.back().mName);
      auto indexedOnField = underlyingClass.GetFieldFromDefinitionName(
          annotation.GetAttribute(Annotations::Index_On)->mValue.mName);
      auto forConverter = annotation.GetAttribute(Annotations::Index_ForConverter);
      if (forConverter == nullptr)
        continue;
      codeBlock.Add("if (converter.{} == nullptr) {{", forConverter->mValue.mName);
      codeBlock.Indent(1);

      Type fromType(mProject, indexedOnField->mField->mDefinitionSource,
                    indexedOnField->mField->mType);
      fromType.PreventCopying();
      auto idField = underlyingClass.GetIdField();
      Type toType(mProject, idField->mField->mDefinitionSource, idField->mField->mType);
      codeBlock.Add("converter.{} = [this]({}key) -> {} {{", forConverter->mValue.mName,
                    fromType.ToString(false), toType.ToString(true));
      codeBlock.Indent(1);
      codeBlock.Add("auto elem = {}(key);",
                    Naming().ContainerIndexGetterNameInCpp(*field.mField, annotation));
      codeBlock.Add("HOLGEN_WARN_AND_RETURN_IF(!elem, {}(-1), \"{{}} {} not found!\", key);",
                    idField->mType.ToString(true),
                    field.mType.mTemplateParameters.back().ToString(true));
      codeBlock.Add("return elem->{}();", Naming().FieldGetterNameInCpp(*idField->mField));
      codeBlock.Indent(-1);
      codeBlock.Add("}};"); // converter =
      codeBlock.Indent(-1);
      codeBlock.Add("}}"); // if (converter == nullptr)
    }
  }

  if (codeBlock.mContents.empty()) {
    method.mBody.Add("auto &converter = converterArg;");
  } else {
    method.mBody.Add("auto converter = converterArg;");
    method.mBody.Add(std::move(codeBlock));
  }
}

void JsonParseFilesPlugin::GenerateFilesByName(ClassMethod &method) {
  method.mBody.Add("std::map<std::string, std::vector<std::filesystem::path>> filesByName;");
  method.mBody.Add("std::queue<std::filesystem::path> pathsQueue;");
  method.mBody.Add("pathsQueue.push(std::filesystem::path(rootPath));");
  method.mBody.Add("while(!pathsQueue.empty()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto& curPath = pathsQueue.front();");
  method.mBody.Add("for (auto &entry: std::filesystem::directory_iterator(curPath)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (std::filesystem::is_directory(entry)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("pathsQueue.push(entry.path());");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else if (std::filesystem::is_regular_file(entry)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (entry.path().extension() == \".json\") {{");
  method.mBody.Indent(1);
  method.mBody.Add("auto filename = entry.path().filename().string();");
  method.mBody.Add("filesByName[filename.substr(0, filename.size() - 5)].push_back(entry.path());");
  method.mBody.Indent(-1);
  method.mBody.Add("}}"); // if (json)
  method.mBody.Indent(-1);
  method.mBody.Add("}}"); // if (regular file)
  method.mBody.Indent(-1);
  method.mBody.Add("}}"); // for entry : curPath
  method.mBody.Add("pathsQueue.pop();");
  method.mBody.Indent(-1);
  method.mBody.Add("}}"); // while(!paths.empty())
}

void JsonParseFilesPlugin::GenerateReadSelf(CodeBlock &codeBlock) {
  codeBlock.Add("if (!selfName.empty()) {{");
  codeBlock.Indent(1);
  codeBlock.Add("auto contents = {}::{}(rootPath / (selfName + \".json\"));", St::FilesystemHelper,
                St::FilesystemHelper_ReadFile);
  codeBlock.Add("rapidjson::Document doc;");
  codeBlock.Add("doc.Parse(contents.c_str());");
  codeBlock.Add("{}(doc, converter{});", St::ParseJson, mLuaStateArgument);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

namespace {
struct FieldDependencyInfo {
  std::set<std::string> mDependedBy = {};
  size_t mNumDependencies = 0;
  const ClassField *mField;

  explicit FieldDependencyInfo(const ClassField &field) : mField(&field) {}
};
} // namespace

std::vector<const ClassField *> JsonParseFilesPlugin::GetProcessOrder(const Class &cls) {
  std::vector<const ClassField *> result;
  std::map<std::string, FieldDependencyInfo> depInfo;
  for (auto &field: cls.mFields) {
    if (!ShouldProcess(field, false))
      continue;
    if (!IsContainerOfDataManager(cls, field))
      continue;
    depInfo.emplace(field.mField->mName, FieldDependencyInfo{field});
  }
  result.reserve(depInfo.size());
  for (auto &[name, info]: depInfo) {
    auto loadBefore = info.mField->mField->GetAnnotation(Annotations::LoadBefore);
    if (loadBefore) {
      for (auto &attrib: loadBefore->mAttributes) {
        auto res = info.mDependedBy.emplace(attrib.mName).second;
        if (res)
          depInfo.at(attrib.mName).mNumDependencies++;
      }
    }
    auto loadAfter = info.mField->mField->GetAnnotation(Annotations::LoadAfter);
    if (loadAfter) {
      for (auto &attrib: loadAfter->mAttributes) {
        auto res = depInfo.at(attrib.mName).mDependedBy.emplace(name).second;
        if (res)
          info.mNumDependencies++;
      }
    }
  }
  while (!depInfo.empty()) {
    bool processed = false;
    auto it = depInfo.begin();
    while (it != depInfo.end()) {
      if (it->second.mNumDependencies == 0) {
        processed = true;
        result.push_back(it->second.mField);
        for (auto &dep: it->second.mDependedBy) {
          depInfo.at(dep).mNumDependencies--;
        }
        it = depInfo.erase(it);
      } else {
        ++it;
      }
    }
    THROW_IF(!processed, "DataManager {} has a circular dependency in load order that includes {}",
             cls.mName, depInfo.begin()->first);
  }
  return result;
}
} // namespace holgen
