#include "HeaderContainer.h"
#include <map>
#include "Translator.h"

namespace holgen {
namespace {
std::set<std::string> CstdIntTypes = {
    "int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
};
std::map<std::string, std::string> STDHeaders = {
    {"std::string", "string"},
    {"std::vector", "vector"},
    {"std::span", "span"},
    {"std::list", "list"},
    {"std::deque", "deque"},
    {"std::map", "map"},
    {"std::unordered_map", "unordered_map"},
    {"std::set", "set"},
    {"std::unordered_set", "unordered_set"},
    {"std::function", "functional"},
    {"std::array", "array"},
    {"std::shared_ptr", "memory"},
    {"std::unique_ptr", "memory"},
    {"std::filesystem::path", "filesystem"},
};
std::set<std::string> NoHeaderTypes = {"float", "double", "void", "bool"};
} // namespace

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

void HeaderContainer::Write(CodeBlock &codeBlock) const {
  auto sortAndInclude = [&](auto &container, const bool useQuotes) {
    std::set<std::string> elems;
    elems.insert(container.begin(), container.end());
    for (const auto &elem: elems) {
      if (useQuotes) {
        codeBlock.Add("#include \"{}\"", elem);
      } else {
        codeBlock.Add("#include <{}>", elem);
      }
    }
  };
  sortAndInclude(mStandardHeaders, false);
  sortAndInclude(mLibHeaders, false);
  sortAndInclude(mLocalHeaders, true);
  if (!mHeaders.empty())
    codeBlock.Line();
  std::map<std::string, std::vector<const ForwardDeclaration *>> fwdDeclarations;
  for (auto &fwdDeclaration: mForwardDeclarations) {
    // already included as a header, don't forward declare
    if (std::find(mLocalHeaders.begin(), mLocalHeaders.end(), fwdDeclaration.mName + ".h") !=
        mLocalHeaders.end())
      continue;
    fwdDeclarations[fwdDeclaration.mNamespace].push_back(&fwdDeclaration);
  }
  for (auto &[_namespace, declarations]: fwdDeclarations) {
    if (!_namespace.empty()) {
      codeBlock.Add("namespace {} {{", _namespace);
      codeBlock.Indent(1);
    }
    for (auto &declaration: declarations) {
      codeBlock.Add("{} {};", declaration->mType, declaration->mName);
    }
    if (!_namespace.empty()) {
      codeBlock.Indent(-1);
      codeBlock.Add("}}");
    }
  }
}

void HeaderContainer::IncludeType(const TranslatedProject &project, const Type &type,
                                  bool isHeader) {
  if (NoHeaderTypes.contains(type.mName)) {
    return;
  } else if (CstdIntTypes.contains(type.mName)) {
    if (isHeader) {
      AddStandardHeader("cstdint");
    }
  } else if (STDHeaders.contains(type.mName)) {
    if (isHeader) {
      AddStandardHeader(STDHeaders.at(type.mName));
    }
  } else if (auto cls = project.GetClass(type.mName)) {
    if (isHeader) {
      if (type.mType == PassByType::Value)
        AddLocalHeader(type.mName + ".h");
      else
        AddForwardDeclaration({cls->mNamespace, "class", cls->mName});
    } else if (type.mType != PassByType::Value) {
      AddLocalHeader(type.mName + ".h");
    }
  }
}

void HeaderContainer::IncludeClassField(const TranslatedProject &project, const Class &cls,
                                        const ClassField &classField, bool isHeader) {
  IncludeClassField(project, cls, classField, classField.mType, isHeader);
}

void HeaderContainer::IncludeClassField(const TranslatedProject &project, const Class &cls,
                                        const ClassField &classField, const Type &type,
                                        bool isHeader) {
  bool isLocalType = false;
  for (const auto &templateParameter: cls.mTemplateParameters) {
    if (templateParameter.mName == type.mName) {
      isLocalType = true;
      break;
    }
  }
  for (const auto &usingStatement: cls.mUsings) {
    if (usingStatement.mTargetType == type.mName) {
      isLocalType = true;
      break;
    }
  }
  if (!isLocalType)
    IncludeType(project, type, isHeader);
  for (const auto &templateParameter: type.mTemplateParameters) {
    IncludeClassField(project, cls, classField, templateParameter, isHeader);
  }
  for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
    IncludeClassField(project, cls, classField, templateParameter, isHeader);
  }
}

void HeaderContainer::IncludeClassMethod(const TranslatedProject &project, const Class &cls,
                                         const ClassMethod &classMethod, bool isHeader) {
  IncludeClassMethod(project, cls, classMethod, classMethod.mReturnType, isHeader);
  for (const auto &argument: classMethod.mArguments) {
    IncludeClassMethod(project, cls, classMethod, argument.mType, isHeader);
  }
}

void HeaderContainer::IncludeClassMethod(const TranslatedProject &project, const Class &cls,
                                         const ClassMethod &method, const Type &type,
                                         bool isHeader) {
  bool isLocalType = cls.GetTemplateParameter(type.mName) ||
      method.GetTemplateParameter(type.mName) || cls.GetUsing(type.mName);
  if (!isLocalType && type.mName != cls.mName)
    IncludeType(project, type, isHeader);
  for (const auto &templateParameter: type.mTemplateParameters) {
    IncludeClassMethod(project, cls, method, templateParameter, isHeader);
  }
  for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
    IncludeClassMethod(project, cls, method, templateParameter, isHeader);
  }
}

void HeaderContainer::IncludeUsing(const TranslatedProject &project, const Using &usingStatement,
                                   bool isHeader) {
  IncludeType(project, usingStatement.mSourceType, isHeader);
}

void HeaderContainer::IncludeBaseClass(const TranslatedProject &project, const Class &cls,
                                         const BaseClass &baseClass) {
  bool isLocalType =
      cls.GetTemplateParameter(baseClass.mType.mName) || cls.GetUsing(baseClass.mType.mName);
  if (!isLocalType)
    IncludeType(project, baseClass.mType, true);
}

void HeaderContainer::AddForwardDeclaration(ForwardDeclaration declaration) {
  mForwardDeclarations.insert(std::move(declaration));
}

void HeaderContainer::Subtract(const HeaderContainer &rhs, std::vector<std::string> &container) {
  std::vector<std::string> temp;
  for (auto &header: container) {
    if (!rhs.mHeaders.contains(header))
      temp.push_back(header);
    else
      mHeaders.erase(header);
  }
  container = std::move(temp);
}

void HeaderContainer::Subtract(const HeaderContainer &rhs) {
  Subtract(rhs, mStandardHeaders);
  Subtract(rhs, mLibHeaders);
  Subtract(rhs, mLocalHeaders);
}
} // namespace holgen
