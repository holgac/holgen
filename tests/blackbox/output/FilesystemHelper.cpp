#include "FilesystemHelper.h"

#include <fstream>

namespace holgen_blackbox_test {
std::string FilesystemHelper::ReadFile(const std::string& filePath) {
  std::ifstream fin(filePath, std::ios_base::binary);
  fin.seekg(0, std::ios_base::end);
  auto bufferSize = fin.tellg();
  bufferSize += 1;
  std::string contents(bufferSize, 0);
  fin.seekg(0, std::ios_base::beg);
  fin.read(contents.data(), contents.size());
  return contents;
}
}
