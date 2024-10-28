#pragma once

#include <format>

#define THROW_IF(cond, msg, ...) \
  { \
    if (cond) { \
      THROW(msg, ##__VA_ARGS__) \
    } \
  }

#define THROW(msg, ...) \
  throw holgen::Exception(std::format("{}:{}: " msg, __FILE__, __LINE__, ##__VA_ARGS__));

#define WARN_IF(cond, msg, ...) \
  { \
    if (cond) { \
      WARN(msg, ##__VA_ARGS__) \
    } \
  }

#define WARN(msg, ...) \
  std::cerr << std::format(msg, ##__VA_ARGS__) << std::endl;


namespace holgen {

class Exception : std::exception {
  std::string mMsg;

public:
  explicit Exception(std::string msg) : mMsg(std::move(msg)) {}

  const char *what() {
    return mMsg.c_str();
  }
};

} // namespace holgen
