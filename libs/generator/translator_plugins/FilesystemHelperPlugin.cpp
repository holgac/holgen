#include "FilesystemHelperPlugin.h"
#include "core/St.h"

namespace holgen {

  void FilesystemHelperPlugin::Run() {
    auto &cls = mProject.mClasses.emplace_back(St::FilesystemHelper);
    cls.mSourceIncludes.AddStandardHeader("fstream");
    auto &readFile = cls.mMethods.emplace_back(St::FilesystemHelper_ReadFile, Type{"std::string"}, Visibility::Public,
                                               Constness::NotConst,
                                               Staticness::Static);
    readFile.mArguments.emplace_back(
        "filePath", Type{"std::string", PassByType::Reference, Constness::Const});
    readFile.mBody.Add("std::ifstream fin(filePath, std::ios_base::binary);");
    readFile.mBody.Add("fin.seekg(0, std::ios_base::end);");
    readFile.mBody.Add("auto bufferSize = fin.tellg();");
    readFile.mBody.Add("bufferSize += 1;");
    readFile.mBody.Add("std::string contents(bufferSize, 0);");
    readFile.mBody.Add("fin.seekg(0, std::ios_base::beg);");
    readFile.mBody.Add("fin.read(contents.data(), contents.size());");
    readFile.mBody.Add("return contents;");
  }
}
