#include "CSharpTypes.h"

namespace holgen {
CSharpMethod *CSharpClass::GetFirstMethod(const std::string &name) {
  for (auto &method: mMethods) {
    if (method.mName == name)
      return &method;
  }
  return nullptr;
}
} // namespace holgen
