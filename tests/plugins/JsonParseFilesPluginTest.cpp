#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/json/JsonParseFilesPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"

class JsonParseFilesPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    JsonConverterPlugin(project, {}).Run();
    JsonParseFilesPlugin(project, {}).Run();
  }

};

TEST_F(JsonParseFilesPluginTest, SimpleStructs) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  string name;
  // @jsonConvert(from=string, using=innerStructNameToId)
  // Ref<InnerStruct2> nestedInnerStruct2;
}
struct InnerStruct2 {
  @id
  u32 id;
  string name;
}
@dataManager
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
  @container(elemName=innerStruct2)
  vector<InnerStruct2> innerStruct2s;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseFiles", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseFiles", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("rootPath", Type{"std::string", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converterArg", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseFiles", Constness::NotConst), method, R"R(
auto &converter = converterArg;
std::map<std::string, std::vector<std::filesystem::path>> filesByName;
std::queue<std::filesystem::path> pathsQueue;
pathsQueue.push(std::filesystem::path(rootPath));
while(!pathsQueue.empty()) {
  auto& curPath = pathsQueue.front();
  for (auto &entry: std::filesystem::directory_iterator(curPath)) {
    if (std::filesystem::is_directory(entry)) {
      pathsQueue.push(entry.path());
    } else if (std::filesystem::is_regular_file(entry)) {
      std::string filename = entry.path().filename();
      auto dotPosition = filename.rfind('.');
      if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == "json") {
        filesByName[filename.substr(0, dotPosition)].push_back(entry.path());
      }
    }
  }
  pathsQueue.pop();
}
auto it = filesByName.find("innerStructs");
if (it != filesByName.end()) {
  for (const auto& filePath: it->second) {
    auto contents = FilesystemHelper::ReadFile(filePath);
    rapidjson::Document doc;
    doc.Parse(contents.c_str());
    HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of InnerStruct entries", filePath.string());
    for (auto& jsonElem: doc.GetArray()) {
      HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
      InnerStruct elem;
      auto res = elem.ParseJson(jsonElem, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
      AddInnerStruct(std::move(elem));
    }
  }
}
it = filesByName.find("innerStruct2s");
if (it != filesByName.end()) {
  for (const auto& filePath: it->second) {
    auto contents = FilesystemHelper::ReadFile(filePath);
    rapidjson::Document doc;
    doc.Parse(contents.c_str());
    HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of InnerStruct2 entries", filePath.string());
    for (auto& jsonElem: doc.GetArray()) {
      HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
      InnerStruct2 elem;
      auto res = elem.ParseJson(jsonElem, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
      AddInnerStruct2(std::move(elem));
    }
  }
}
return true;
    )R");
  }
}

TEST_F(JsonParseFilesPluginTest, IndexedStructsWithDependencies) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  string name;
  @jsonConvert(from=string, using=innerStruct2NameToId)
  Ref<InnerStruct2> nestedInnerStruct2;
}
struct InnerStruct2 {
  @id
  u32 id;
  string name;
}
@dataManager
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
  @index(on=name, forConverter=innerStruct2NameToId)
  @container(elemName=innerStruct2)
  vector<InnerStruct2> innerStruct2s;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseFiles", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseFiles", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("rootPath", Type{"std::string", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converterArg", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseFiles", Constness::NotConst), method, R"R(
auto converter = converterArg;
if (converter.innerStruct2NameToId == nullptr) {
  converter.innerStruct2NameToId = [this](const std::string &key) -> uint32_t {
    auto elem = GetInnerStruct2FromName(key);
    HOLGEN_WARN_AND_RETURN_IF(!elem, uint32_t(-1), "{} InnerStruct2 not found!", key);
    return elem->GetId();
  };
}
std::map<std::string, std::vector<std::filesystem::path>> filesByName;
std::queue<std::filesystem::path> pathsQueue;
pathsQueue.push(std::filesystem::path(rootPath));
while(!pathsQueue.empty()) {
  auto& curPath = pathsQueue.front();
  for (auto &entry: std::filesystem::directory_iterator(curPath)) {
    if (std::filesystem::is_directory(entry)) {
      pathsQueue.push(entry.path());
    } else if (std::filesystem::is_regular_file(entry)) {
      std::string filename = entry.path().filename();
      auto dotPosition = filename.rfind('.');
      if (dotPosition != std::string::npos && filename.substr(dotPosition + 1) == "json") {
        filesByName[filename.substr(0, dotPosition)].push_back(entry.path());
      }
    }
  }
  pathsQueue.pop();
}
auto it = filesByName.find("innerStruct2s");
if (it != filesByName.end()) {
  for (const auto& filePath: it->second) {
    auto contents = FilesystemHelper::ReadFile(filePath);
    rapidjson::Document doc;
    doc.Parse(contents.c_str());
    HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of InnerStruct2 entries", filePath.string());
    for (auto& jsonElem: doc.GetArray()) {
      HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
      InnerStruct2 elem;
      auto res = elem.ParseJson(jsonElem, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
      AddInnerStruct2(std::move(elem));
    }
  }
}
it = filesByName.find("innerStructs");
if (it != filesByName.end()) {
  for (const auto& filePath: it->second) {
    auto contents = FilesystemHelper::ReadFile(filePath);
    rapidjson::Document doc;
    doc.Parse(contents.c_str());
    HOLGEN_WARN_AND_RETURN_IF(!doc.IsArray(), false, "Invalid json file {}: It is supposed to contain a list of InnerStruct entries", filePath.string());
    for (auto& jsonElem: doc.GetArray()) {
      HOLGEN_WARN_AND_CONTINUE_IF(!jsonElem.IsObject(), "Invalid entry in json file {}", filePath.string());
      InnerStruct elem;
      auto res = elem.ParseJson(jsonElem, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Invalid entry in json file {}", filePath.string());
      AddInnerStruct(std::move(elem));
    }
  }
}
return true;
    )R");
  }
}
