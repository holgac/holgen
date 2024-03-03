#include <gtest/gtest.h>
#include "generator/generators/GeneratorGlobalPointer.h"
#include "generator/CodeGenerator.h"
#include "Helpers.h"

using namespace holgen;

namespace {

  TEST(GeneratorGlobalPointerTest, HeaderAndSource) {
    ProjectDefinition pd;
    TranslatedProject tp(pd);
    GeneratorGlobalPointer ggp(pd, tp);
    ggp.GenerateHelpers();
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
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
