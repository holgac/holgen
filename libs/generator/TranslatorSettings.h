#pragma once

namespace holgen {
namespace TranslatorFeatureFlag {
enum Entry : uint64_t {
  Lua = 1,
  Json = 2,
};
}; // namespace TranslatorFeatureFlag

// This is read from CLI args
struct TranslatorSettings {
  std::string mNamespace;
  // unless overridden with an annotation, this is the default when comparing floats
  std::string mEpsilon = "0.00001";

  TranslatorFeatureFlag::Entry mFeatureFlags = TranslatorFeatureFlag::Entry(0);

  [[nodiscard]] bool IsFeatureEnabled(const TranslatorFeatureFlag::Entry featureFlag) const {
    return (mFeatureFlags & featureFlag) == featureFlag;
  }

  void EnableFeature(const TranslatorFeatureFlag::Entry featureFlag) {
    mFeatureFlags = TranslatorFeatureFlag::Entry(mFeatureFlags | featureFlag);
  }
};
} // namespace holgen
