#include "DotNetHost.h"

#include <unistd.h>
#include <vector>

using namespace holgen_blackbox_csharp;

int main() {
  std::vector<char> pathBuffer(4096, 0);
  getcwd(pathBuffer.data(), pathBuffer.size() - 1);
  std::filesystem::path mPathToBinFolder = pathBuffer.data();
  mPathToBinFolder = mPathToBinFolder / "bin";
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  return module1.PrimitiveReturnVector();
}
