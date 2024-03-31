#pragma once

#include <format>

#define THROW_IF(cond, msg, ...) { if (cond) { \
  THROW(msg, ## __VA_ARGS__) \
  } }

#define THROW(msg, ...) throw Exception(std::format("{}:{}: " msg, __FILE__, __LINE__, ## __VA_ARGS__));

namespace holgen {

  class Exception : std::exception {
    std::string mMsg;
  public:
    explicit Exception(std::string msg) : mMsg(std::move(msg)) {}

    const char *what() { return mMsg.c_str(); }
  };

}
