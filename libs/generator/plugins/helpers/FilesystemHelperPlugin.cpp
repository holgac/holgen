#include "FilesystemHelperPlugin.h"
#include "core/St.h"

namespace holgen {

void FilesystemHelperPlugin::Run() {
  auto cls = Class{St::FilesystemHelper, mSettings.mNamespace};
  cls.mSourceIncludes.AddStandardHeader("fstream");
  // TODO: use std::filesystem::path instead of std::string
  auto method = ClassMethod{
      St::FilesystemHelper_ReadFile, Type{"std::string"}, Visibility::Public,
      Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back(
      "filePath", Type{"std::string", PassByType::Reference, Constness::Const});
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
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}
}
