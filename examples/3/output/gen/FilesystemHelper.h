// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <filesystem>
#include <string>

namespace ex3_schemas {
class FilesystemHelper {
public:
  static std::string ReadFile(const std::filesystem::path &filePath);
};
}
