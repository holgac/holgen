#include <gtest/gtest.h>
#include "generator/Generator.h"

using namespace holgen;

namespace {

  std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents) {
    std::map<std::string, GeneratedContent> result;
    for (const auto &gc: contents) {
      auto[it, res] = result.emplace(gc.mName, gc);
      EXPECT_TRUE(res);
    }
    return result;
  }

  auto Trim(const std::string &str) {
    auto startIdx = str.find_first_not_of(" \n");
    auto endIdx = str.find_last_not_of(" \n");
    return std::string_view(str.data() + startIdx, endIdx - startIdx);
  }

  void ExpectGeneratedContent(const GeneratedContent &lhs, const GeneratedContent &rhs) {
    EXPECT_EQ(lhs.mType, rhs.mType);
    EXPECT_EQ(lhs.mName, rhs.mName);
    EXPECT_EQ(Trim(lhs.mText), Trim(rhs.mText));
  }

  TEST(GeneratorTest, ClassWithGetters) {
    Tokenizer tokenizer(R"DELIM(
  struct Person   {
    u32 age;
    float gender;
  }
    )DELIM");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator().Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
    };
    Generator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    EXPECT_EQ(files.size(), 3);
    ExpectGeneratedContent(
        files["CMakeLists.txt"],
        {
            FileType::CMakeFile,
            "CMakeLists.txt",
            "add_library(generator_test_cmake Person.cpp)"
        }
    );
    ExpectGeneratedContent(
        files["Person.h"],
        {
            FileType::CppHeader,
            "Person.h",
            R"DELIM(
#pragma once

#include <cstdint>

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetAge() const;
  void SetAge(uint32_t val);
  float GetGender() const;
  void SetGender(float val);
protected:
private:
  uint32_t mAge;
  float mGender;
};
}
)DELIM"
        }
    );
    ExpectGeneratedContent(
        files["Person.cpp"],
        {
            FileType::CppSource,
            "Person.cpp",
            R"DELIM(
#include "Person.h"

namespace generator_test_namespace {
uint32_t Person::GetAge() const {
  return mAge;
}
void Person::SetAge(uint32_t val) {
  mAge = val;
}
float Person::GetGender() const {
  return mGender;
}
void Person::SetGender(float val) {
  mGender = val;
}
}
          )DELIM"
        }
    );
  }
}
