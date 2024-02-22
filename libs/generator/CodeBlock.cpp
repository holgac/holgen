#include "CodeBlock.h"
#include "Translator.h"

namespace holgen {
  void CodeBlock::Add(const CodeBlock &cb) {
    mContents.insert(mContents.end(), cb.mContents.begin(), cb.mContents.end());
    mLines.insert(mLines.end(), cb.mLines.begin(), cb.mLines.end());
    mIndentations.insert(mIndentations.end(), cb.mIndentations.begin(), cb.mIndentations.end());
  }

  std::string CodeBlock::ToString() const {
    ssize_t currentIndentation = 0;
    auto contentIt = mContents.begin();
    auto lineIt = mLines.begin();
    auto indentIt = mIndentations.begin();
    std::string indentation;
    std::stringstream out;
    for (; contentIt != mContents.end(); ++contentIt) {
      switch (*contentIt) {
        case CodeUnitType::Indentation:
          currentIndentation += *indentIt;
          indentation = std::string(currentIndentation * 2, ' ');
          ++indentIt;
          break;
          case CodeUnitType::Code:
            out << indentation << *lineIt << std::endl;
            ++lineIt;
            break;
      }
    }
    if (currentIndentation != 0) {
      throw GeneratorException("Inconsistent indentation!");
    }
    return out.str();
  }
}
