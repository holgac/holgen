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

  void ExpectGeneratedContent(const GeneratedContent &actual, const GeneratedContent &expected) {
    EXPECT_EQ(actual.mType, expected.mType);
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(Trim(actual.mText), Trim(expected.mText));
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
    ExpectGeneratedContent(
        files["CMakeLists.txt"],
        {
            FileType::CMakeFile,
            "CMakeLists.txt",
            "add_library(generator_test_cmake Person.cpp JsonHelper.cpp Converter.cpp)"
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
#include <rapidjson/document.h>
#include "Converter.h"

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetAge() const;
  void SetAge(uint32_t val);
  float GetGender() const;
  void SetGender(float val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
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

#include "JsonHelper.h"

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
bool Person::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "age")) {
      auto res = JsonHelper::Parse(mAge, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "gender")) {
      auto res = JsonHelper::Parse(mGender, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
          )DELIM"
        }
    );
  }

  TEST(GeneratorTest, ClassWithContainers) {
    Tokenizer tokenizer(R"DELIM(
  struct Market {
    vector<string> instruments;
    map<string, double> prices;
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
    ExpectGeneratedContent(
        files["Market.h"],
        {
            FileType::CppHeader,
            "Market.h",
            R"DELIM(
#pragma once

#include <vector>
#include <string>
#include <map>
#include <rapidjson/document.h>
#include "Converter.h"

namespace generator_test_namespace {
class Market {
public:
  const std::vector<std::string>& GetInstruments() const;
  std::vector<std::string>& GetInstruments();
  void SetInstruments(const std::vector<std::string>& val);
  const std::map<std::string, double>& GetPrices() const;
  std::map<std::string, double>& GetPrices();
  void SetPrices(const std::map<std::string, double>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  std::vector<std::string> mInstruments;
  std::map<std::string, double> mPrices;
};
}
          )DELIM"
        }
    );
    ExpectGeneratedContent(
        files["Market.cpp"],
        {
            FileType::CppSource,
            "Market.cpp",
            R"DELIM(
#include "Market.h"

#include "JsonHelper.h"

namespace generator_test_namespace {
const std::vector<std::string>& Market::GetInstruments() const {
  return mInstruments;
}
std::vector<std::string>& Market::GetInstruments() {
  return mInstruments;
}
void Market::SetInstruments(const std::vector<std::string>& val) {
  mInstruments = val;
}
const std::map<std::string, double>& Market::GetPrices() const {
  return mPrices;
}
std::map<std::string, double>& Market::GetPrices() {
  return mPrices;
}
void Market::SetPrices(const std::map<std::string, double>& val) {
  mPrices = val;
}
bool Market::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "instruments")) {
      auto res = JsonHelper::Parse(mInstruments, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "prices")) {
      auto res = JsonHelper::Parse(mPrices, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
          )DELIM"
        }
    );
  }

  TEST(GeneratorTest, NestedClasses) {
    Tokenizer tokenizer(R"DELIM(
  struct Sound {
    string name;
    u32 volume;
  }
  struct Animal {
    vector<Sound> sounds;
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
    ExpectGeneratedContent(
        files["Sound.h"],
        {
            FileType::CppHeader,
            "Sound.h",
            R"DELIM(
#pragma once

#include <string>
#include <cstdint>
#include <rapidjson/document.h>
#include "Converter.h"

namespace generator_test_namespace {
class Sound {
public:
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  uint32_t GetVolume() const;
  void SetVolume(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  std::string mName;
  uint32_t mVolume;
};
}
          )DELIM"
        }
    );

    ExpectGeneratedContent(
        files["Sound.cpp"],
        {
            FileType::CppSource,
            "Sound.cpp",
            R"DELIM(
#include "Sound.h"

#include "JsonHelper.h"

namespace generator_test_namespace {
const std::string& Sound::GetName() const {
  return mName;
}
std::string& Sound::GetName() {
  return mName;
}
void Sound::SetName(const std::string& val) {
  mName = val;
}
uint32_t Sound::GetVolume() const {
  return mVolume;
}
void Sound::SetVolume(uint32_t val) {
  mVolume = val;
}
bool Sound::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "name")) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "volume")) {
      auto res = JsonHelper::Parse(mVolume, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
          )DELIM"
        }
    );


    ExpectGeneratedContent(
        files["Animal.h"],
        {
            FileType::CppHeader,
            "Animal.h",
            R"DELIM(
#pragma once

#include <vector>
#include <rapidjson/document.h>
#include "Sound.h"
#include "Converter.h"

namespace generator_test_namespace {
class Animal {
public:
  const std::vector<Sound>& GetSounds() const;
  std::vector<Sound>& GetSounds();
  void SetSounds(const std::vector<Sound>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  std::vector<Sound> mSounds;
};
}
          )DELIM"
        }
    );

    ExpectGeneratedContent(
        files["Animal.cpp"],
        {
            FileType::CppSource,
            "Animal.cpp",
            R"DELIM(
#include "Animal.h"

#include "JsonHelper.h"

namespace generator_test_namespace {
const std::vector<Sound>& Animal::GetSounds() const {
  return mSounds;
}
std::vector<Sound>& Animal::GetSounds() {
  return mSounds;
}
void Animal::SetSounds(const std::vector<Sound>& val) {
  mSounds = val;
}
bool Animal::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "sounds")) {
      auto res = JsonHelper::Parse(mSounds, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
          )DELIM"
        }
    );

  }

  TEST(GeneratorTest, Converters) {
    Tokenizer tokenizer(R"DELIM(
  struct Person {
    @jsonConvert(from=string, using=countryToId)
    u32 currentCountry;
    @jsonConvert(from=string, using=cityToId)
    u32 currentCity;
    @jsonConvert(from=string, using=countryToId)
    u32 homeCountry;
    @jsonConvert(from=string, using=cityToId)
    u32 placeOfBirth;
  }
  struct Country {
    Person leader;
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
    ExpectGeneratedContent(
        files["Person.h"],
        {
          FileType::CppHeader,
          "Person.h",
          R"DELIM(
#pragma once

#include <cstdint>
#include <rapidjson/document.h>
#include "Converter.h"

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetCurrentCountry() const;
  void SetCurrentCountry(uint32_t val);
  uint32_t GetCurrentCity() const;
  void SetCurrentCity(uint32_t val);
  uint32_t GetHomeCountry() const;
  void SetHomeCountry(uint32_t val);
  uint32_t GetPlaceOfBirth() const;
  void SetPlaceOfBirth(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  uint32_t mCurrentCountry;
  uint32_t mCurrentCity;
  uint32_t mHomeCountry;
  uint32_t mPlaceOfBirth;
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

#include "JsonHelper.h"

namespace generator_test_namespace {
uint32_t Person::GetCurrentCountry() const {
  return mCurrentCountry;
}
void Person::SetCurrentCountry(uint32_t val) {
  mCurrentCountry = val;
}
uint32_t Person::GetCurrentCity() const {
  return mCurrentCity;
}
void Person::SetCurrentCity(uint32_t val) {
  mCurrentCity = val;
}
uint32_t Person::GetHomeCountry() const {
  return mHomeCountry;
}
void Person::SetHomeCountry(uint32_t val) {
  mHomeCountry = val;
}
uint32_t Person::GetPlaceOfBirth() const {
  return mPlaceOfBirth;
}
void Person::SetPlaceOfBirth(uint32_t val) {
  mPlaceOfBirth = val;
}
bool Person::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "currentCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCountry = converter.countryToId(temp);
    } else if (0 == strcmp(name, "currentCity")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mCurrentCity = converter.cityToId(temp);
    } else if (0 == strcmp(name, "homeCountry")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mHomeCountry = converter.countryToId(temp);
    } else if (0 == strcmp(name, "placeOfBirth")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mPlaceOfBirth = converter.cityToId(temp);
    }
  }
  return true;
}
}
          )DELIM"
        }
        );
    ExpectGeneratedContent(
        files["Country.h"],
        {
            FileType::CppHeader,
            "Country.h",
            R"DELIM(
#pragma once

#include <rapidjson/document.h>
#include "Person.h"
#include "Converter.h"

namespace generator_test_namespace {
class Country {
public:
  const Person& GetLeader() const;
  Person& GetLeader();
  void SetLeader(const Person& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
protected:
private:
  Person mLeader;
};
}
          )DELIM"
        }
    );
    ExpectGeneratedContent(
        files["Country.cpp"],
        {
            FileType::CppSource,
            "Country.cpp",
            R"DELIM(
#include "Country.h"

#include "JsonHelper.h"

namespace generator_test_namespace {
const Person& Country::GetLeader() const {
  return mLeader;
}
Person& Country::GetLeader() {
  return mLeader;
}
void Country::SetLeader(const Person& val) {
  mLeader = val;
}
bool Country::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "leader")) {
      auto res = mLeader.ParseJson(data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
}
          )DELIM"
        }
    );
  }

}
