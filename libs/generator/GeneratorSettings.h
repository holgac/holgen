#pragma once
#include "FileType.h"
#include "CodeBlock.h"
#include <string>

namespace holgen {

struct GeneratedContent {
  FileType mType;
  std::string mName;
  CodeBlock mBody;
};

namespace GeneratorFeatureFlag {
enum Entry : uint64_t {
};
}; // namespace GeneratorFeatureFlag

// This is read from CLI args
struct GeneratorSettings {
  std::string mProjectName;
  std::string mConfigHeader;
  GeneratorFeatureFlag::Entry mFeatureFlags = GeneratorFeatureFlag::Entry(0);

  [[nodiscard]] bool IsFeatureEnabled(const GeneratorFeatureFlag::Entry featureFlag) const {
    return (mFeatureFlags & featureFlag) != 0;
  }

  void EnableFeature(const GeneratorFeatureFlag::Entry featureFlag) {
    mFeatureFlags = GeneratorFeatureFlag::Entry(mFeatureFlags | featureFlag);
  }
};
} // namespace holgen
