#pragma once
#include <format>
#include "core/Exception.h"

namespace holgen {

enum class Constexprness {
  Constexpr,
  NotConstexpr,
};

enum class Visibility {
  Private,
  Protected,
  Public,
};

enum class Staticness {
  Static,
  NotStatic,
};

enum class Noexceptness {
  Noexcept,
  NotNoexcept,
};

enum class Explicitness {
  Explicit,
  NotExplicit,
};

enum class DefaultDelete {
  Default,
  Delete,
  Neither,
};

enum class Virtuality {
  NotVirtual,
  Virtual,
  PureVirtual,
};
} // namespace holgen

namespace std {
template <>
struct formatter<holgen::Visibility> : formatter<std::string> {
  auto format(const holgen::Visibility &visibility, format_context &ctx) const {
    switch (visibility) {
    case holgen::Visibility::Public:
      return std::format_to(ctx.out(), "public");
    case holgen::Visibility::Protected:
      return std::format_to(ctx.out(), "protected");
    case holgen::Visibility::Private:
      return std::format_to(ctx.out(), "private");
    }
    THROW("Unexpected visibility: {}", uint32_t(visibility));
  }
};
} // namespace std
