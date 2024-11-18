#include "JsonDumpFilesPlugin.h"
#include <vector>
#include "generator/utils/NamingConvention.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/TypeInfo.h"

namespace holgen {
namespace {
std::string ParseJson = "ParseJson";
std::string DumpFiles = "DumpFiles";
} // namespace

void JsonDumpFilesPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    if (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Json) ||
        cls.mStruct->GetAnnotation(Annotations::DataManager) == nullptr)
      continue;
    GenerateDumpFiles(cls);
  }
}

void JsonDumpFilesPlugin::GenerateDumpFiles(Class &cls) {
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/stringbuffer.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/writer.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");

  cls.mHeaderIncludes.AddStandardHeader("filesystem");
  cls.mSourceIncludes.AddStandardHeader("queue");
  cls.mSourceIncludes.AddStandardHeader("vector");
  cls.mSourceIncludes.AddStandardHeader("fstream");
  cls.mSourceIncludes.AddLocalHeader(St::FilesystemHelper + ".h");
  auto method = ClassMethod{DumpFiles, Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back(
      "rootPath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("selfName",
                                 Type{"std::string", PassByType::Reference, Constness::Const});

  method.mBody.Add("if (std::filesystem::exists(rootPath)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("std::filesystem::remove_all(rootPath);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("std::filesystem::create_directories(rootPath);");
  method.mBody.Add("rapidjson::Document doc;");
  GenerateDumpSelf(method.mBody);

  for (auto &field: cls.mFields) {
    if (!IsContainerOfDataManager(cls, field))
      continue;
    GenerateDumpContainerField(field, method.mBody);
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonDumpFilesPlugin::GenerateDumpSelf(CodeBlock &codeBlock) {
  codeBlock.Add("{}::{}(rootPath / (selfName + \".json\"), {}(doc));", St::JsonHelper,
                St::JsonHelper_DumpToFile, St::DumpJson);
}

void JsonDumpFilesPlugin::GenerateDumpContainerField(const ClassField &field,
                                                     CodeBlock &codeBlock) {
  codeBlock.Add("{0}::{1}(rootPath / \"{2}\", {0}::{3}({4}, doc));", St::JsonHelper,
                St::JsonHelper_DumpToFile, field.mField->mName + ".json", St::JsonHelper_Dump, field.mName);
}

} // namespace holgen
