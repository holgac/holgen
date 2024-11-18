#include "FilesystemHelperPlugin.h"
#include "core/St.h"

namespace holgen {

void FilesystemHelperPlugin::Run() {
  auto &cls = GenerateClass();
  GenerateReadFile(cls);
  GenerateDumpFile(cls);
}

Class &FilesystemHelperPlugin::GenerateClass() const {

  auto cls = Class{St::FilesystemHelper, mSettings.mNamespace};
  cls.mHeaderIncludes.AddStandardHeader("filesystem");
  cls.mSourceIncludes.AddStandardHeader("fstream");
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
  return mProject.mClasses.back();
}

void FilesystemHelperPlugin::GenerateReadFile(Class &cls) const {
  auto method = ClassMethod{St::FilesystemHelper_ReadFile, Type{"std::string"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back(
      "filePath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mBody.Add("std::ifstream fin(filePath, std::ios_base::binary);");
  method.mBody.Add("fin.seekg(0, std::ios_base::end);");
  method.mBody.Add("auto bufferSize = fin.tellg();");
  method.mBody.Add("bufferSize += 1;");
  method.mBody.Add("std::string contents(bufferSize, 0);");
  method.mBody.Add("fin.seekg(0, std::ios_base::beg);");
  method.mBody.Add("fin.read(contents.data(), contents.size());");
  method.mBody.Add("return contents;");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void FilesystemHelperPlugin::GenerateDumpFile(Class &cls) const {
  auto method = ClassMethod{St::FilesystemHelper_DumpFile, Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back(
      "filePath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("contents",
                                 Type{"std::string", PassByType::Reference, Constness::Const});
  method.mBody.Add("std::ofstream out(filePath, std::ios_base::binary);");
  method.mBody.Add("out.write(contents.c_str(), contents.size());");

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
