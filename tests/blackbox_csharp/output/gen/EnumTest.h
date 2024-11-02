// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <format>

namespace holgen_blackbox_csharp {
// Defined in tests/blackbox_csharp/schemas/EnumTest.hsc
class EnumTest {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Entry1 = 3,
    Entry2 = 0,
    Entry5 = 1,
  };
  explicit EnumTest(UnderlyingType value = Invalid);
  EnumTest(Entry value);
  EnumTest::Entry GetValue() const;
  static EnumTest FromString(std::string_view str);
  const char *ToString() const;
  EnumTest &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const EnumTest &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const EnumTest &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const EnumTest &rhs) const;
  constexpr static std::array<EnumTest::Entry, 3> GetEntries() {
    return std::array<EnumTest::Entry, 3>{Entry1, Entry2, Entry5};
  }
  static constexpr const UnderlyingType Invalid = 2;
private:
  EnumTest::Entry mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_csharp::EnumTest> {
public:
  size_t operator()(const holgen_blackbox_csharp::EnumTest &obj) const {
    return std::hash<holgen_blackbox_csharp::EnumTest::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_csharp::EnumTest::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_csharp::EnumTest::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_csharp::EnumTest(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_csharp::EnumTest> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_csharp::EnumTest &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}