#pragma once

#include <string>
#include <vector>
#include <set>


namespace holgen {

  struct TranslatedProject;
  struct Class;
  struct ClassMethod;
  struct ClassField;
  struct Type;
  struct Typedef;
  struct CodeBlock;

  class HeaderContainer {
    std::set<std::string> mHeaders;
    std::vector<std::string> mStandardHeaders;
    std::vector<std::string> mLibHeaders;
    std::vector<std::string> mLocalHeaders;
    void IncludeClassField(const TranslatedProject &project, const Class &cls, const ClassField &classField,
                           const Type &type, bool isHeader);
    void IncludeClassMethod(const TranslatedProject &project, const Class &cls, const ClassMethod &classMethod,
                            const Type &type, bool isHeader);
    void IncludeType(const TranslatedProject &project, const Type &type, bool isHeader);
  public:
    void AddStandardHeader(const std::string &header);

    void AddLibHeader(const std::string &header);

    void AddLocalHeader(const std::string &header);

    void Write(CodeBlock &codeBlock);


    void IncludeTypedef(const TranslatedProject &project, const Class &cls, const Typedef& typdef, bool isHeader);

    void
    IncludeClassField(const TranslatedProject &project, const Class &cls, const ClassField &classField, bool isHeader);
    void IncludeClassMethod(const TranslatedProject &project, const Class &cls, const ClassMethod &classMethod,
                            bool isHeader);
  };
}