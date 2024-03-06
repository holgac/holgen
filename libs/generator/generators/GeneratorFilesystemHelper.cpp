#include "GeneratorFilesystemHelper.h"
#include "core/St.h"

namespace holgen {

  void GeneratorFilesystemHelper::GenerateHelpers() {
    auto &cls = mTranslatedProject.mClasses.emplace_back();
    cls.mName = St::FilesystemHelper;
    cls.mSourceIncludes.AddStandardHeader("fstream");
    auto &readFile = cls.mMethods.emplace_back();
    readFile.mName = St::FilesystemHelper_ReadFile;
    readFile.mStaticness = Staticness::Static;
    readFile.mConstness = Constness::NotConst;
    readFile.mReturnType.mName = "std::string";
    auto& arg = readFile.mArguments.emplace_back();
    arg.mName = "filePath";
    arg.mType.mName = "std::string";
    arg.mType.mType = PassByType::Reference;
    arg.mType.mConstness = Constness::Const;
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
