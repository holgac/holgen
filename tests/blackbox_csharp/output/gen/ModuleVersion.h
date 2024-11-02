// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include "DeferredDeleter.h"
#include "EnumTest.h"

namespace holgen_blackbox_csharp {
// Defined in tests/blackbox_csharp/schemas/DotNetModule.hsc
class ModuleVersion {
public:
  ModuleVersion(const uint16_t major, const uint16_t minor, const uint32_t revision);
  ModuleVersion() = default;
  ModuleVersion(const ModuleVersion &rhs) = default;
  ModuleVersion(ModuleVersion &&rhs) noexcept = default;
  ~ModuleVersion() = default;
  uint16_t GetMajor() const;
  uint16_t GetMinor() const;
  uint32_t GetRevision() const;
  void SetMajor(uint16_t val);
  void SetMinor(uint16_t val);
  void SetRevision(uint32_t val);
  static ModuleVersion Construct(const uint16_t major, const uint16_t minor, const uint32_t revision);
  static ModuleVersion ConstructDouble(const uint16_t major, const uint16_t minor, const uint32_t revision);
  uint32_t GetSum() const;
  void BumpRevision();
  ModuleVersion SumOfSelfAnd(const ModuleVersion &v1, const ModuleVersion &v2);
  std::vector<ModuleVersion> GetNextRevisions(const uint64_t count);
  std::array<ModuleVersion, 3> GetNextThreeRevisions();
  std::vector<uint32_t> GetDataAsVector();
  std::array<uint32_t, 3> GetDataAsArray();
  std::vector<int32_t> GetDataAsSignedVector();
  std::vector<EnumTest> GetEnums(const std::vector<EnumTest> &value);
  bool operator==(const ModuleVersion &rhs) const;
  ModuleVersion &operator=(const ModuleVersion &rhs) = default;
  ModuleVersion &operator=(ModuleVersion &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "ModuleVersion";
private:
  uint16_t mMajor;
  uint16_t mMinor;
  uint32_t mRevision;
};
}
extern "C" {
  HOLGEN_EXPORT holgen_blackbox_csharp::ModuleVersion holgen_blackbox_csharp_ModuleVersion_Construct(const uint16_t major, const uint16_t minor, const uint32_t revision);
  HOLGEN_EXPORT holgen_blackbox_csharp::ModuleVersion holgen_blackbox_csharp_ModuleVersion_ConstructDouble(const uint16_t major, const uint16_t minor, const uint32_t revision);
  HOLGEN_EXPORT uint32_t holgen_blackbox_csharp_ModuleVersion_GetSum(const holgen_blackbox_csharp::ModuleVersion *instance);
  HOLGEN_EXPORT void holgen_blackbox_csharp_ModuleVersion_BumpRevision(holgen_blackbox_csharp::ModuleVersion *instance);
  HOLGEN_EXPORT holgen_blackbox_csharp::ModuleVersion holgen_blackbox_csharp_ModuleVersion_SumOfSelfAnd(holgen_blackbox_csharp::ModuleVersion *instance, const holgen_blackbox_csharp::ModuleVersion *v1, const holgen_blackbox_csharp::ModuleVersion *v2);
  HOLGEN_EXPORT holgen_blackbox_csharp::ModuleVersion *holgen_blackbox_csharp_ModuleVersion_GetNextRevisions(holgen_blackbox_csharp::ModuleVersion *instance, const uint64_t count, size_t *holgenReturnValueHolgenSize, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
  HOLGEN_EXPORT holgen_blackbox_csharp::ModuleVersion *holgen_blackbox_csharp_ModuleVersion_GetNextThreeRevisions(holgen_blackbox_csharp::ModuleVersion *instance, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
  HOLGEN_EXPORT uint32_t *holgen_blackbox_csharp_ModuleVersion_GetDataAsVector(holgen_blackbox_csharp::ModuleVersion *instance, size_t *holgenReturnValueHolgenSize, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
  HOLGEN_EXPORT uint32_t *holgen_blackbox_csharp_ModuleVersion_GetDataAsArray(holgen_blackbox_csharp::ModuleVersion *instance, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
  HOLGEN_EXPORT int32_t *holgen_blackbox_csharp_ModuleVersion_GetDataAsSignedVector(holgen_blackbox_csharp::ModuleVersion *instance, size_t *holgenReturnValueHolgenSize, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
  HOLGEN_EXPORT holgen_blackbox_csharp::EnumTest *holgen_blackbox_csharp_ModuleVersion_GetEnums(holgen_blackbox_csharp::ModuleVersion *instance, holgen_blackbox_csharp::EnumTest *value, size_t valueHolgenSize, size_t *holgenReturnValueHolgenSize, holgen_blackbox_csharp::DeferredDeleter **holgenDeferredDeleter);
}