#include "StringSwitcher.h"

namespace holgen {

StringSwitcher::StringSwitcher(std::string sourceStr, CodeBlock elseCase) :
    mSourceStr(std::move(sourceStr)), mElseCase(std::move(elseCase)) {}

void StringSwitcher::AddCase(StringSwitchCase aCase) { mCases.push_back(std::move(aCase)); }

CodeBlock StringSwitcher::Generate() {
  CodeBlock codeBlock;
  bool isFirst = true;
  for (auto &caseItem: mCases) {
    if (isFirst) {
      codeBlock.Add("if (0 == strcmp(\"{}\", {})) {{", caseItem.mCase, mSourceStr);
      isFirst = false;
    } else {
      codeBlock.Add("}} else if (0 == strcmp(\"{}\", {})) {{", caseItem.mCase, mSourceStr);
    }
    codeBlock.Indent(1);
    caseItem.action(codeBlock);
    codeBlock.Indent(-1);
  }
  if (mElseCase.mContents.empty())
    codeBlock.Add("}}");
  else if (!mCases.empty()) {
    codeBlock.Add("}} else {{");
    codeBlock.Indent(1);
    codeBlock.Add(std::move(mElseCase));
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
  return codeBlock;
}

bool StringSwitcher::IsEmpty() const { return mCases.empty(); }
} // namespace holgen
