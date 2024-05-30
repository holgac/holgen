#include "JsonParseFilesPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "../../NamingConvention.h"

namespace holgen {
namespace {
std::string ParseJson = "ParseJson";
std::string ParseFiles = "ParseFiles";
}

void JsonParseFilesPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    if (cls.mStruct->GetAnnotation(Annotations::NoJson) ||
        cls.mStruct->GetAnnotation(Annotations::DataManager) == nullptr)
      continue;
    GenerateParseFiles(cls);
  }
}

void JsonParseFilesPlugin::GenerateParseFiles(Class &cls) {
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");

  cls.mSourceIncludes.AddStandardHeader("filesystem");
  cls.mSourceIncludes.AddStandardHeader("queue");
  cls.mSourceIncludes.AddStandardHeader("vector");
  cls.mSourceIncludes.AddLocalHeader(St::FilesystemHelper + ".h");
  auto method = ClassMethod{ParseFiles, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("rootPath", Type{"std::string", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converterArg", Type{St::Converter, PassByType::Reference, Constness::Const});

  GenerateConverterPopulators(cls, method);
  GenerateFilesByName(method);

  bool isFirst = true;
  for (const auto &structToProcess : mProject.mDependencyGraph.GetProcessOrder()) {
    for (const auto &field: cls.mFields) {
      if (!field.mField || !field.mField->GetAnnotation(Annotations::Container))
        continue;
      auto &fieldDefinition = *field.mField;
      auto &templateParameter = fieldDefinition.mType.mTemplateParameters[0];
      if (templateParameter.mName != structToProcess)
        continue;

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
          structToProcess);
      method.mBody.Add("for (auto& jsonElem: doc.GetArray()) {{"); // if (!doc.IsArray())
      method.mBody.Indent(1);
      method.mBody.Add(
          R"(HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {{}}", filePath.string());)");
      Type type(mProject, templateParameter);
      method.mBody.Add("{}elem;", type.ToString(false)); // if (!doc.IsArray())
      method.mBody.Add("auto res = elem.{}(jsonElem, converter);", ParseJson); // if (!doc.IsArray())
      method.mBody.Add(
          R"(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {{}}", filePath.string());)");
      method.mBody.Add("{}(std::move(elem));", Naming().ContainerElemAdderNameInCpp(fieldDefinition));
      method.mBody.Indent(-1);
      method.mBody.Add("}}"); // for (jsonElem: doc.GetArray())

      method.mBody.Indent(-1);
      method.mBody.Add("}}"); // for(path: filesByName[field])

      method.mBody.Indent(-1);
      method.mBody.Add("}}"); // if (it != filesByName.end())
    }
  }
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
      auto &underlyingClass = *mProject.GetClass(field.mField->mType.mTemplateParameters.back().mName);
      auto indexedOnField = underlyingClass.GetFieldFromDefinitionName(
          annotation.GetAttribute(Annotations::Index_On)->mValue.mName);
      auto forConverter = annotation.GetAttribute(Annotations::Index_ForConverter);
      if (forConverter == nullptr)
        continue;
      codeBlock.Add("if (converter.{} == nullptr) {{", forConverter->mValue.mName);
      codeBlock.Indent(1);

      Type fromType(mProject, indexedOnField->mField->mType);
      fromType.PreventCopying();
      auto idField = underlyingClass.GetIdField();
      Type toType(mProject, idField->mField->mType);
      codeBlock.Add("converter.{} = [this]({}key) -> {} {{", forConverter->mValue.mName, fromType.ToString(false),
                    toType.ToString(true));
      codeBlock.Indent(1);
      codeBlock.Add("auto elem = {}(key);", Naming().ContainerIndexGetterNameInCpp(*field.mField, annotation));
      codeBlock.Add("HOLGEN_WARN_AND_RETURN_IF(!elem, {}(-1), \"{{}} {} not found!\", key);",
                    idField->mType.ToString(true), field.mType.mTemplateParameters.back().ToString(true));
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
  method.mBody.Add("std::string filename = entry.path().filename().string();");
  method.mBody.Add("auto dotPosition = filename.rfind('.');");
  method.mBody.Add("if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == \"json\") {{");
  method.mBody.Indent(1);
  method.mBody.Add("filesByName[filename.substr(0, dotPosition)].push_back(entry.path());");
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
}
