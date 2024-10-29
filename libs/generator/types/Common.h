#pragma once

namespace holgen {

enum class PassByType {
  Value,
  Reference,
  Pointer,
  MoveReference,
};

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
