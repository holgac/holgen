#pragma once

#include "holgen.h"
#include <string>

namespace holgen_blackbox_test {
class FilesystemHelper {
public:
  static std::string ReadFile(const std::string& filePath);
protected:
private:
};
}