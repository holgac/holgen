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
  struct Using;
  struct CodeBlock;

  struct ForwardDeclaration {
    std::string mNamespace;
    std::string mType;
    std::string mName;

    bool operator<(const ForwardDeclaration &other) const {
      if (mName == other.mName)
        return mNamespace < other.mNamespace;
      return mName < other.mName;
    }
  };

  class HeaderContainer {
  public:
    void AddStandardHeader(const std::string &header);
    void AddLibHeader(const std::string &header);
    void AddLocalHeader(const std::string &header);
    void AddForwardDeclaration(ForwardDeclaration declaration);
    void Write(CodeBlock &codeBlock) const;
    void IncludeUsing(const TranslatedProject &project, const Using &usingStatement, bool isHeader);
    void IncludeClassField(
        const TranslatedProject &project, const Class &cls, const ClassField &classField, bool isHeader);
    void IncludeClassMethod(
        const TranslatedProject &project, const Class &cls, const ClassMethod &classMethod, bool isHeader);
    void Subtract(const HeaderContainer &rhs);
  private:
    std::set<std::string> mHeaders;
    std::vector<std::string> mStandardHeaders;
    std::vector<std::string> mLibHeaders;
    std::vector<std::string> mLocalHeaders;
    std::set<ForwardDeclaration> mForwardDeclarations;
    void IncludeClassField(const TranslatedProject &project, const Class &cls, const ClassField &classField,
                           const Type &type, bool isHeader);
    void IncludeClassMethod(const TranslatedProject &project, const Class &cls, const ClassMethod &method,
                            const Type &type, bool isHeader);
    void IncludeType(const TranslatedProject &project, const Type &type, bool isHeader);
    void Subtract(const HeaderContainer &rhs, std::vector<std::string> &container);
  };
}