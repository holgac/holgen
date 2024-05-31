#pragma once


#include <map>
#include "CodeBlock.h"

namespace holgen {
struct StringSwitchCase {
  std::string mCase;
  std::function<void(CodeBlock &)> action;
};

class StringSwitcher {
public:
  StringSwitcher(std::string sourceStr, CodeBlock elseCase = {});


  template <typename... Args>
  void AddCase(Args... args) {
    AddCase(StringSwitchCase{std::forward<Args>(args)...});
  }

  void AddCase(StringSwitchCase aCase);
  CodeBlock Generate();
  bool IsEmpty() const;

private:
  std::vector<StringSwitchCase> mCases;
  std::string mSourceStr;
  CodeBlock mElseCase;
};

} // namespace holgen
