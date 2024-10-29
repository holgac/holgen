#include "DotNetHost.h"
#include "SingletonCalculator.h"

#include <unistd.h>
#include <vector>

using namespace holgen_blackbox_csharp;

int main() {
  std::vector<char> pathBuffer(4096, 0);
  getcwd(pathBuffer.data(), pathBuffer.size() - 1);
  std::filesystem::path pathToBinFolder = pathBuffer.data();
  pathToBinFolder = pathToBinFolder / "bin";
  SingletonCalculator::SetSingleton(new SingletonCalculator());
  DotNetHost dotNetHost;
  dotNetHost.Initialize(pathToBinFolder / "CSharpBindings");
  auto &calc = SingletonCalculator::GetInstance();
  return 0;
}

int main3() {
  std::vector<char> pathBuffer(4096, 0);
  getcwd(pathBuffer.data(), pathBuffer.size() - 1);
  std::filesystem::path pathToBinFolder = pathBuffer.data();
  pathToBinFolder = pathToBinFolder / "bin";
  SingletonCalculator::SetSingleton(new SingletonCalculator());
  DotNetHost dotNetHost;
  dotNetHost.Initialize(pathToBinFolder / "CSharpBindings");
  auto &module1 = dotNetHost.LoadCustomDotNetModule(pathToBinFolder / "TestModule");
  module1.Initialize();
  auto &calc = SingletonCalculator::GetInstance();
  return 0;
}
