#pragma once


#include <map>
#include <set>
#include <string>
#include "generator/plugins/TranslatorPlugin.h"

namespace holgen {
class JsonHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void GenerateParseJsonForSmartPointer(Class &cls, const std::string &pointerType,
                                        const std::string &pointerGenerator);
  void GenerateParseJsonForSingleElemContainer(Class &cls, const std::string &container,
                                               bool withConverter);
  void GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container,
                                          bool withKeyConverter, bool withElemConverter);
  void GenerateBaseParse(Class &cls);
  void GenerateParseSingleElem(Class &cls);
  void GenerateParseSingleElem(Class &cls, const std::string &type, const std::string &validator,
                               const std::string &getter);
  void GenerateParseTuple(Class &cls, size_t size, const std::string &tupleClassName);
};
} // namespace holgen
