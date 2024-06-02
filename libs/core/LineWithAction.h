#pragma once

#include <functional>
#include <sstream>
#include <string>

namespace holgen {

class LineWithAction {
private:
  std::stringstream mStream;
  std::function<void(const std::string &)> mAction;

public:
  LineWithAction(std::function<void(const std::string &)> action) : mAction(std::move(action)) {}

  LineWithAction(LineWithAction &&ll) :
      mStream(std::move(ll.mStream)), mAction(std::move(ll.mAction)) {}

  ~LineWithAction() {
    mAction(mStream.str());
  }

  template <typename T>
  LineWithAction &operator<<(const T &val) {
    mStream << val;
    return *this;
  }
};
} // namespace holgen
