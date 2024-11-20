// This file is generated by holgen. Do not modify manually.
#pragma once

#include <iostream>
#include <format>
#include "../config.h"

#ifndef HOLGEN_FAIL
#define HOLGEN_FAIL(msg, ...) throw std::runtime_error(std::format("{}:{} " msg, __FILE__, __LINE__, ## __VA_ARGS__))
#endif // ifndef HOLGEN_FAIL

#ifndef HOLGEN_FAIL_IF
#define HOLGEN_FAIL_IF(cond, msg, ...) if (cond) { \
HOLGEN_FAIL(msg, ## __VA_ARGS__); \
}
#endif // ifndef HOLGEN_FAIL_IF

#ifndef HOLGEN_WARN
#define HOLGEN_WARN(msg, ...) std::cerr << std::format("{}:{} " msg, __FILE__, __LINE__, ## __VA_ARGS__) << std::endl
#endif // ifndef HOLGEN_WARN

#ifndef HOLGEN_WARN_IF
#define HOLGEN_WARN_IF(cond, msg, ...) if (cond) { \
HOLGEN_WARN(msg, ## __VA_ARGS__); \
}
#endif // ifndef HOLGEN_WARN_IF

#ifndef HOLGEN_WARN_AND_RETURN_IF
#define HOLGEN_WARN_AND_RETURN_IF(cond, retVal, msg, ...) if (cond) { \
HOLGEN_WARN(msg, ## __VA_ARGS__); \
return retVal; \
}
#endif // ifndef HOLGEN_WARN_AND_RETURN_IF

#ifndef HOLGEN_WARN_AND_CONTINUE_IF
#define HOLGEN_WARN_AND_CONTINUE_IF(cond, msg, ...) if (cond) { \
HOLGEN_WARN(msg, ## __VA_ARGS__); \
continue; \
}
#endif // ifndef HOLGEN_WARN_AND_CONTINUE_IF
#if WINDOWS
#define HOLGEN_EXPORT __declspec(dllexport)
#elif __GNUC__ >= 4
#define HOLGEN_EXPORT __attribute__ ((visibility ("default")))
#else
#define HOLGEN_EXPORT
#endif
namespace holgen_blackbox_test {
template <typename T>
concept EnumConcept = requires(T t) {
  typename T::UnderlyingType;
  std::is_convertible_v<typename T::UnderlyingType, int64_t>;
  {
    T::Invalid
  } -> std::convertible_to<int64_t>;
  {
    t.GetValue()
  } -> std::same_as<typename T::Entry>;
};
}
