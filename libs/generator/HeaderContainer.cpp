#include "HeaderContainer.h"
#include <map>

namespace holgen {
  namespace {
    std::set<std::string> CstdIntTypes = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
    };
    std::map<std::string, std::string> STLContainers = {
        {"std::string",        "string"},
        {"std::vector",        "vector"},
        {"std::map",           "map"},
        {"std::unordered_map", "unordered_map"},
    };
    std::set<std::string> NoHeaderTypes = {"float", "double"};
  }

  void HeaderContainer::AddStandardHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mStandardHeaders.push_back(header);
  }

  void HeaderContainer::AddLibHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mLibHeaders.push_back(header);
  }

  void HeaderContainer::AddLocalHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mLocalHeaders.push_back(header);
  }

  void HeaderContainer::Write(CodeBlock &codeBlock) {
    for (const auto &header: mStandardHeaders)
      codeBlock.Line() << "#include <" << header << ">";
    for (const auto &header: mLibHeaders)
      codeBlock.Line() << "#include <" << header << ">";
    for (const auto &header: mLocalHeaders)
      codeBlock.Line() << "#include \"" << header << "\"";
    if (!mHeaders.empty())
      codeBlock.Line();
  }

  void HeaderContainer::AddForType(const Type &type, bool isHeader) {
    if (NoHeaderTypes.contains(type.mName)) {
      return;
    } else if (CstdIntTypes.contains(type.mName)) {
      if (isHeader) {
        AddStandardHeader("cstdint");
      }
    } else if (STLContainers.contains(type.mName)) {
      if (isHeader) {
        AddStandardHeader(STLContainers.at(type.mName));
      }
    } else if (type.mName.starts_with("rapidjson::")) {
      // it's better to fwd declare in header, but rapidjson uses templates making it complicated
      if (isHeader) {
        AddLibHeader("rapidjson/document.h");
      }
    }
  }
}
