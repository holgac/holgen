#include "CSharpWrapperGenerator.h"

#include "core/St.h"

namespace holgen {
void CSharpWrapperGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  (void)contents;
}
} // namespace holgen
