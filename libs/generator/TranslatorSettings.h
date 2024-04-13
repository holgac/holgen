#pragma once

namespace holgen {
  // This is read from CLI args
  struct TranslatorSettings {
    std::string mNamespace;
    // unless overridden with an annotation, this is the default when comparing floats
    std::string mEpsilon = "0.00001";
  };
}
