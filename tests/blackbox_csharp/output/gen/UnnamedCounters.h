// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <algorithm>
#include <vector>
#include "Counter.h"
#include "DeferredDeleter.h"

namespace holgen_blackbox_csharp {
// Defined in tests/blackbox_csharp/schemas/CounterManager.hsc
class UnnamedCounters {
public:
  UnnamedCounters() = default;
  UnnamedCounters(const UnnamedCounters &rhs) = default;
  UnnamedCounters(UnnamedCounters &&rhs) noexcept = default;
  ~UnnamedCounters() = default;
  const std::vector<Counter> &GetCounters() const;
  std::vector<Counter> &GetCounters();
  bool operator==(const UnnamedCounters &rhs) const;
  UnnamedCounters &operator=(const UnnamedCounters &rhs) = default;
  UnnamedCounters &operator=(UnnamedCounters &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "UnnamedCounters";
private:
  void SetCounters(const std::vector<Counter> &val);
  std::vector<Counter> mCounters;
};
}
extern "C" {
  HOLGEN_EXPORT holgen_blackbox_csharp::Counter *holgen_blackbox_csharp_UnnamedCounters_GetCounters(const holgen_blackbox_csharp::UnnamedCounters *instance, size_t *holgenReturnValueHolgenSize, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
}