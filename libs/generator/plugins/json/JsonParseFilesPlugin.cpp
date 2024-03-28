#include "JsonParseFilesPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "../../Naming.h"

namespace holgen {
  namespace {
    std::string ConverterName = "Converter";
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
    const auto &structDefinition = *mProject.mProject.GetStruct(cls.mName);
    auto &parseFunc = cls.mMethods.emplace_back(
        ParseFiles, Type{"bool"},
        Visibility::Public, Constness::NotConst);
    parseFunc.mArguments.emplace_back("rootPath", Type{"std::string", PassByType::Reference, Constness::Const});
    parseFunc.mArguments.emplace_back("converterArg", Type{ConverterName, PassByType::Reference, Constness::Const});
    // TODO: do this only when necessary
    parseFunc.mBody.Add("auto converter = converterArg;");
    for (const auto &fieldDefinition: structDefinition.mFields) {
      if (!fieldDefinition.GetAnnotation(Annotations::Container))
        continue;
      for (const auto &annotationDefinition: fieldDefinition.mAnnotations) {
        if (annotationDefinition.mName != Annotations::Index)
          continue;
        auto &underlyingStruct = *mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
        auto indexedOnField = underlyingStruct.GetField(
            annotationDefinition.GetAttribute(Annotations::Index_On)->mValue.mName);
        auto forConverter = annotationDefinition.GetAttribute(Annotations::Index_ForConverter);
        if (forConverter == nullptr)
          continue;
        parseFunc.mBody.Add("if (converter.{} == nullptr) {{", forConverter->mValue.mName);
        parseFunc.mBody.Indent(1);

        Type fromType(mProject.mProject, indexedOnField->mType);
        fromType.PreventCopying();
        auto idField = underlyingStruct.GetIdField();
        Type toType(mProject.mProject, idField->mType);
        parseFunc.mBody.Add("converter.{} = [this]({} key) -> {} {{", forConverter->mValue.mName, fromType.ToString(),
                            toType.ToString());
        parseFunc.mBody.Indent(1);

        parseFunc.mBody.Add("auto elem = {}(key);",
                            Naming(mProject).ContainerIndexGetterNameInCpp(fieldDefinition, annotationDefinition));
        parseFunc.mBody.Add("return elem->{}();", Naming(mProject).FieldGetterNameInCpp(*idField));

        parseFunc.mBody.Indent(-1);
        parseFunc.mBody.Add("}};"); // converter =

        parseFunc.mBody.Indent(-1);
        parseFunc.mBody.Add("}}"); // if (converter == nullptr)

      }
    }

    parseFunc.mBody.Add("std::map<std::string, std::vector<std::filesystem::path>> filesByName;");
    parseFunc.mBody.Add("std::queue<std::filesystem::path> pathsQueue;");
    parseFunc.mBody.Add("pathsQueue.push(std::filesystem::path(rootPath));");
    parseFunc.mBody.Add("while(!pathsQueue.empty()) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("auto& curPath = pathsQueue.front();");
    parseFunc.mBody.Add("for (auto &entry: std::filesystem::directory_iterator(curPath)) {{");
    parseFunc.mBody.Indent(1);

    parseFunc.mBody.Add("if (std::filesystem::is_directory(entry)) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("pathsQueue.push(entry.path());");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}} else if (std::filesystem::is_regular_file(entry)) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("std::string filename = entry.path().filename();");
    parseFunc.mBody.Add("auto dotPosition = filename.rfind('.');");
    parseFunc.mBody.Add("if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == \"json\") {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("filesByName[filename.substr(0, dotPosition)].push_back(entry.path());");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // if (json)

    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // if (regular file)

    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // for entry : curPath
    parseFunc.mBody.Add("pathsQueue.pop();");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}"); // while(!paths.empty())

    bool isFirst = true;
    for (const auto &structToProcess : mProject.mDependencyGraph.GetProcessOrder()) {
      for (const auto &fieldDefinition: structDefinition.mFields) {
        if (!fieldDefinition.GetAnnotation(Annotations::Container))
          continue;
        auto &templateParameter = fieldDefinition.mType.mTemplateParameters[0];
        if (templateParameter.mName == structToProcess) {
          {
            auto line = parseFunc.mBody.Line();
            if (isFirst) {
              line << "auto it";
              isFirst = false;
            } else {
              line << "it";
            }
            line << " = filesByName.find(\"" << fieldDefinition.mName << "\");";
          }
          parseFunc.mBody.Add("if (it != filesByName.end()) {{");
          parseFunc.mBody.Indent(1);

          parseFunc.mBody.Add("for (const auto& filePath: it->second) {{");
          parseFunc.mBody.Indent(1);
          parseFunc.mBody.Add("auto contents = {}::{}(filePath);", St::FilesystemHelper,
                              St::FilesystemHelper_ReadFile);
          parseFunc.mBody.Add("rapidjson::Document doc;");
          parseFunc.mBody.Add("doc.Parse(contents.c_str());");
          parseFunc.mBody.Add(
              R"(HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {{}}: It is supposed to contain a list of {} entries", filePath.string());)",
              structToProcess);
          parseFunc.mBody.Add("for (auto& jsonElem: doc.GetArray()) {{"); // if (!doc.IsArray())
          parseFunc.mBody.Indent(1);
          parseFunc.mBody.Add(
              R"(HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {{}}", filePath.string());)");
          Type type(mProject.mProject, templateParameter);
          parseFunc.mBody.Add("{} elem;", type.ToString()); // if (!doc.IsArray())
          parseFunc.mBody.Add("auto res = elem.{}(jsonElem, converter);", ParseJson); // if (!doc.IsArray())
          parseFunc.mBody.Add(
              R"(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {{}}", filePath.string());)");
          parseFunc.mBody.Add("{}(std::move(elem));", Naming(mProject).ContainerElemAdderNameInCpp(fieldDefinition));
          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // for (jsonElem: doc.GetArray())

          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // for(path: filesByName[field])

          parseFunc.mBody.Indent(-1);
          parseFunc.mBody.Add("}}"); // if (it != filesByName.end())
        }
      }
    }
    parseFunc.mBody.Add("return true;");
  }
}
