// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <format>
#include <rapidjson/fwd.h>

struct lua_State;
namespace ex2_schemas {
  class Converter;
}
namespace ex2_schemas {
// enums can have comments too
class Race {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Human = 2,
    // enum entries can have comments too
    Elf = 0,
    Dwarf = 1,
    Hobbit = 3,
  };
  explicit Race(UnderlyingType value = Invalid);
  Race(Entry value);
  Race::Entry GetValue() const;
  static Race FromString(std::string_view str);
  const char *ToString() const;
  Race &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const Race &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const Race &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const Race &rhs) const;
  constexpr static std::array<Race::Entry, 4> GetEntries() {
    return std::array<Race::Entry, 4>{Human, Elf, Dwarf, Hobbit};
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Race ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void PushEnumToLua(lua_State *luaState);
  inline static constexpr const UnderlyingType Invalid = 4;
private:
  Race::Entry mValue;
};
}
namespace std {
template <>
struct hash<ex2_schemas::Race> {
public:
  size_t operator()(const ex2_schemas::Race &obj) const {
    return std::hash<ex2_schemas::Race::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<ex2_schemas::Race::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const ex2_schemas::Race::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", ex2_schemas::Race(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<ex2_schemas::Race> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const ex2_schemas::Race &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
