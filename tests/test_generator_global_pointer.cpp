#include <gtest/gtest.h>
#include "generator/translator_plugins/GlobalPointerPlugin.h"
#include "generator/CodeGenerator.h"
#include "Helpers.h"

using namespace holgen;

namespace {

  TEST(GeneratorGlobalPointerTest, HeaderAndSource) {
    ProjectDefinition pd;
    TranslatedProject tp(pd);
    GlobalPointerPlugin ggp(tp);
    ggp.GenerateHelpers();
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = helpers::MapByName(generator.Generate(tp));
    helpers::ExpectGeneratedContent(
        files["GlobalPointer.h"],
        {
            FileType::CppHeader,
            "GlobalPointer.h",
            R"DELIM(
#pragma once

#include "holgen.h"
namespace generator_test_namespace {
template <typename T>
class GlobalPointer {
public:
  static T* GetInstance() {
    return mInstance;
  }
  static void SetInstance(T* ptr) {
    mInstance = ptr;
  }
protected:
private:
  inline static T* mInstance = nullptr;
};
}
)DELIM"
        }
    );
    // TODO: test helpers
  }
}
