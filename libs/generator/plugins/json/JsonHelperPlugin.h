#pragma once


#include <map>
#include <set>
#include <string>
#include "JsonPluginBase.h"

namespace holgen {
class JsonHelperPlugin : public JsonPluginBase {
public:
  using JsonPluginBase::JsonPluginBase;
  void Run() override;

private:
  void GenerateParseFunctions(Class &cls);
  void GenerateParseFromFile(Class &cls);
  void GenerateParseJsonForSmartPointer(Class &cls, const std::string &pointerType,
                                        const std::string &pointerGenerator);
  void GenerateParseJsonForSingleElemContainer(Class &cls, const std::string &container,
                                               bool withConverter);
  void GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container,
                                          bool withKeyConverter, bool withElemConverter);
  void GenerateBaseParse(Class &cls);
  void GenerateParseSingleElem(Class &cls);
  void GenerateParseJsonForKeyedContainerElem(CodeBlock& codeBlock, const std::string &container, bool withKeyConverter, bool withElemConverter, const std::string& keyVariable, const std::string& valueVariable);
  void GenerateParseSingleElem(Class &cls, const std::string &type, const std::string &validator,
                               const std::string &getter);
  void GenerateParseTuple(Class &cls, size_t size, const std::string &tupleClassName);
  void GenerateParseLuaObject(Class &cls);
  void GenerateParseLuaRegistryObject(Class &cls);
  void GenerateDumpToFile(Class &cls);
  void GenerateDumpFunctions(Class &cls);
  void GenerateDumpLuaObject(Class &cls);
  void GenerateDumpLuaRegistryObject(Class &cls);
  void GenerateDumpSingleElem(Class &cls);
  void GenerateBaseDump(Class &cls);
  ClassMethod GenerateDumpMethod(const std::string &type);
  void GenerateDumpForSingleElemContainer(Class &cls, const std::string &container);
  void GenerateDumpForKeyedContainer(Class &cls, const std::string &container);
  void GenerateDumpTuple(Class &cls, size_t size, const std::string &container);

  [[nodiscard]] std::string GetIsStringTypeCondition(const std::string &keyTemplateParameter) const {
    return std::format("std::same_as<{}, std::string>", keyTemplateParameter);
  }
};
} // namespace holgen
