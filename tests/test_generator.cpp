#include <gtest/gtest.h>
#include "Helpers.h"
#include "generator/CodeGenerator.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"

using namespace holgen;
using namespace holgen::helpers;

namespace {

TEST(GeneratorTest, Helpers) {
  Tokenizer tokenizer(R"R(
  @no(lua)
  struct Person   {
    @jsonConvert(from=string, using=stringToU32Converter)
    u32 age;
    float gender;
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Lua);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));
  ExpectGeneratedContent(files["CMakeLists.txt"], FileType::CMakeFile, "CMakeLists.txt",
                         R"R(
# This file is partially generated by holgen. Only modify the custom sections.
set(gen_sources
    gen/Person.cpp
    gen/Converter.cpp
    gen/JsonHelper.cpp
    gen/LuaHelper.cpp
    gen/GlobalPointer.cpp
    gen/FilesystemHelper.cpp
    gen/Singleton.cpp
)
set(src_sources
)
set(custom_sources)
# HOLGEN_USER_DEFINED_BEGIN:CustomSources
# HOLGEN_USER_DEFINED_END:CustomSources
add_library(generator_test_cmake STATIC ${gen_sources} ${src_sources} ${custom_sources})
# HOLGEN_USER_DEFINED_BEGIN:CustomDependencies
# HOLGEN_USER_DEFINED_END:CustomDependencies
if (UNIX)
  target_compile_options(generator_test_cmake PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable)
endif ()
)R");
  // TODO: test helpers
}

TEST(GeneratorTest, ClassWithGetters) {
  Tokenizer tokenizer(R"R(
  @no(lua)
  @no(json)
  @comment('Person class')
  @comment('Represents a person')
  struct Person   {
    @comment('this is the age of the person')
    u32 age = 0;
    @comment('gender of the person', "The value should be between 0 and 1")
    @epsilon('0.1')
    float gender;
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));
  ExpectGeneratedContent(files["gen/Person.h"], FileType::CppHeader, "gen/Person.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>

namespace generator_test_namespace {
/*
 * Person class
 * Represents a person
 */
// Defined in GeneratorTest
class Person {
public:
  Person() = default;
  Person(const Person &rhs) = default;
  Person(Person &&rhs) noexcept = default;
  ~Person() = default;
  uint32_t GetAge() const;
  float GetGender() const;
  void SetAge(uint32_t val);
  void SetGender(float val);
  bool operator==(const Person &rhs) const;
  Person &operator=(const Person &rhs) = default;
  Person &operator=(Person &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Person";
private:
  // this is the age of the person
  uint32_t mAge = 0;
  /*
   * gender of the person
   * The value should be between 0 and 1
   */
  float mGender;
};
}
)R");
  ExpectGeneratedContent(files["gen/Person.cpp"], FileType::CppSource, "gen/Person.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include <cmath>

namespace generator_test_namespace {
uint32_t Person::GetAge() const {
  return mAge;
}

float Person::GetGender() const {
  return mGender;
}

void Person::SetAge(uint32_t val) {
  mAge = val;
}

void Person::SetGender(float val) {
  mGender = val;
}

bool Person::operator==(const Person &rhs) const {
  return !(
      mAge != rhs.mAge ||
      std::fabs(mGender - rhs.mGender) >= 0.1
  );
}
}
          )R");
}

TEST(GeneratorTest, ClassWithContainers) {
  Tokenizer tokenizer(R"R(
  @no(lua, json)
  struct Market {
    vector<string> instruments;
    map<string, double> prices;
    @comment('Callback when a new trade is executed')
    func OnNewTrade(string instrument, double price);
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));
  ExpectGeneratedContent(files["gen/Market.h"], FileType::CppHeader, "gen/Market.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <map>
#include <string>
#include <vector>

namespace generator_test_namespace {
// Defined in GeneratorTest
class Market {
public:
  Market() = default;
  Market(const Market &rhs) = default;
  Market(Market &&rhs) noexcept = default;
  ~Market() = default;
  const std::vector<std::string> &GetInstruments() const;
  std::vector<std::string> &GetInstruments();
  const std::map<std::string, double> &GetPrices() const;
  std::map<std::string, double> &GetPrices();
  void SetInstruments(const std::vector<std::string> &val);
  void SetPrices(const std::map<std::string, double> &val);
  // Callback when a new trade is executed
  void OnNewTrade(const std::string &instrument, const double price);
  bool operator==(const Market &rhs) const;
  Market &operator=(const Market &rhs) = default;
  Market &operator=(Market &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Market";
private:
  std::vector<std::string> mInstruments;
  std::map<std::string, double> mPrices;
};
}
          )R");
  ExpectGeneratedContent(files["gen/Market.cpp"], FileType::CppSource, "gen/Market.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Market.h"

namespace generator_test_namespace {
const std::vector<std::string> &Market::GetInstruments() const {
  return mInstruments;
}

std::vector<std::string> &Market::GetInstruments() {
  return mInstruments;
}

const std::map<std::string, double> &Market::GetPrices() const {
  return mPrices;
}

std::map<std::string, double> &Market::GetPrices() {
  return mPrices;
}

void Market::SetInstruments(const std::vector<std::string> &val) {
  mInstruments = val;
}

void Market::SetPrices(const std::map<std::string, double> &val) {
  mPrices = val;
}

bool Market::operator==(const Market &rhs) const {
  return !(
      mInstruments != rhs.mInstruments ||
      mPrices != rhs.mPrices
  );
}
}
          )R");
}

TEST(GeneratorTest, NestedClasses) {
  Tokenizer tokenizer(R"R(
  @no(lua, json)
  struct Sound {
    string name;
    u32 volume;
  }
  @no(lua, json)
  struct Animal {
    vector<Sound> sounds;
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));
  ExpectGeneratedContent(files["gen/Sound.h"], FileType::CppHeader, "gen/Sound.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>

namespace generator_test_namespace {
// Defined in GeneratorTest
class Sound {
public:
  Sound() = default;
  Sound(const Sound &rhs) = default;
  Sound(Sound &&rhs) noexcept = default;
  ~Sound() = default;
  const std::string &GetName() const;
  std::string &GetName();
  uint32_t GetVolume() const;
  void SetName(const std::string &val);
  void SetVolume(uint32_t val);
  bool operator==(const Sound &rhs) const;
  Sound &operator=(const Sound &rhs) = default;
  Sound &operator=(Sound &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Sound";
private:
  std::string mName;
  uint32_t mVolume;
};
}
          )R");

  ExpectGeneratedContent(files["gen/Sound.cpp"], FileType::CppSource, "gen/Sound.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Sound.h"

namespace generator_test_namespace {
const std::string &Sound::GetName() const {
  return mName;
}

std::string &Sound::GetName() {
  return mName;
}

uint32_t Sound::GetVolume() const {
  return mVolume;
}

void Sound::SetName(const std::string &val) {
  mName = val;
}

void Sound::SetVolume(uint32_t val) {
  mVolume = val;
}

bool Sound::operator==(const Sound &rhs) const {
  return !(
      mName != rhs.mName ||
      mVolume != rhs.mVolume
  );
}
}
          )R");


  ExpectGeneratedContent(files["gen/Animal.h"], FileType::CppHeader, "gen/Animal.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <vector>
#include "Sound.h"

namespace generator_test_namespace {
// Defined in GeneratorTest
class Animal {
public:
  Animal() = default;
  Animal(const Animal &rhs) = default;
  Animal(Animal &&rhs) noexcept = default;
  ~Animal() = default;
  const std::vector<Sound> &GetSounds() const;
  std::vector<Sound> &GetSounds();
  void SetSounds(const std::vector<Sound> &val);
  bool operator==(const Animal &rhs) const;
  Animal &operator=(const Animal &rhs) = default;
  Animal &operator=(Animal &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Animal";
private:
  std::vector<Sound> mSounds;
};
}
          )R");

  ExpectGeneratedContent(files["gen/Animal.cpp"], FileType::CppSource, "gen/Animal.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Animal.h"

namespace generator_test_namespace {
const std::vector<Sound> &Animal::GetSounds() const {
  return mSounds;
}

std::vector<Sound> &Animal::GetSounds() {
  return mSounds;
}

void Animal::SetSounds(const std::vector<Sound> &val) {
  mSounds = val;
}

bool Animal::operator==(const Animal &rhs) const {
  return !(
      mSounds != rhs.mSounds
  );
}
}
          )R");
}

TEST(GeneratorTest, Converters) {
  Tokenizer tokenizer(R"R(
  @no(lua)
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
  @no(lua)
  struct Country {
    @no(json)
    string name;
    Person leader;
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));
  ExpectGeneratedContent(files["gen/Person.h"], FileType::CppHeader, "gen/Person.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>

namespace generator_test_namespace {
  class Converter;
}
namespace generator_test_namespace {
// Defined in GeneratorTest
class Person {
public:
  Person() = default;
  Person(const Person &rhs) = default;
  Person(Person &&rhs) noexcept = default;
  ~Person() = default;
  uint32_t GetCurrentCountry() const;
  uint32_t GetCurrentCity() const;
  uint32_t GetHomeCountry() const;
  uint32_t GetPlaceOfBirth() const;
  void SetCurrentCountry(uint32_t val);
  void SetCurrentCity(uint32_t val);
  void SetHomeCountry(uint32_t val);
  void SetPlaceOfBirth(uint32_t val);
  bool operator==(const Person &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  Person &operator=(const Person &rhs) = default;
  Person &operator=(Person &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Person";
private:
  uint32_t mCurrentCountry;
  uint32_t mCurrentCity;
  uint32_t mHomeCountry;
  uint32_t mPlaceOfBirth;
};
}
          )R");
  ExpectGeneratedContent(files["gen/Person.cpp"], FileType::CppSource, "gen/Person.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"

namespace generator_test_namespace {
uint32_t Person::GetCurrentCountry() const {
  return mCurrentCountry;
}

uint32_t Person::GetCurrentCity() const {
  return mCurrentCity;
}

uint32_t Person::GetHomeCountry() const {
  return mHomeCountry;
}

uint32_t Person::GetPlaceOfBirth() const {
  return mPlaceOfBirth;
}

void Person::SetCurrentCountry(uint32_t val) {
  mCurrentCountry = val;
}

void Person::SetCurrentCity(uint32_t val) {
  mCurrentCity = val;
}

void Person::SetHomeCountry(uint32_t val) {
  mHomeCountry = val;
}

void Person::SetPlaceOfBirth(uint32_t val) {
  mPlaceOfBirth = val;
}

bool Person::operator==(const Person &rhs) const {
  return !(
      mCurrentCountry != rhs.mCurrentCountry ||
      mCurrentCity != rhs.mCurrentCity ||
      mHomeCountry != rhs.mHomeCountry ||
      mPlaceOfBirth != rhs.mPlaceOfBirth
  );
}

bool Person::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("currentCountry", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCountry field");
        mCurrentCountry = converter.countryToId(temp);
      } else if (0 == strcmp("currentCity", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCity field");
        mCurrentCity = converter.cityToId(temp);
      } else if (0 == strcmp("homeCountry", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.homeCountry field");
        mHomeCountry = converter.countryToId(temp);
      } else if (0 == strcmp("placeOfBirth", name)) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.placeOfBirth field");
        mPlaceOfBirth = converter.cityToId(temp);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Person: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCountry field");
      mCurrentCountry = converter.countryToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.currentCity field");
      mCurrentCity = converter.cityToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.homeCountry field");
      mHomeCountry = converter.countryToId(temp);
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Person!");
      std::string temp;
      auto res = JsonHelper::Parse(temp, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Person.placeOfBirth field");
      mPlaceOfBirth = converter.cityToId(temp);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Person!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Person.");
    return false;
  }
  return true;
}

rapidjson::Value Person::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("currentCountry", JsonHelper::Dump(mCurrentCountry, doc), doc.GetAllocator());
  val.AddMember("currentCity", JsonHelper::Dump(mCurrentCity, doc), doc.GetAllocator());
  val.AddMember("homeCountry", JsonHelper::Dump(mHomeCountry, doc), doc.GetAllocator());
  val.AddMember("placeOfBirth", JsonHelper::Dump(mPlaceOfBirth, doc), doc.GetAllocator());
  return val;
}
}
          )R");
  ExpectGeneratedContent(files["gen/Country.h"], FileType::CppHeader, "gen/Country.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <string>
#include <rapidjson/fwd.h>
#include "Person.h"

namespace generator_test_namespace {
  class Converter;
}
namespace generator_test_namespace {
// Defined in GeneratorTest
class Country {
public:
  Country() = default;
  Country(const Country &rhs) = default;
  Country(Country &&rhs) noexcept = default;
  ~Country() = default;
  const std::string &GetName() const;
  std::string &GetName();
  const Person &GetLeader() const;
  Person &GetLeader();
  void SetName(const std::string &val);
  void SetLeader(const Person &val);
  bool operator==(const Country &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  Country &operator=(const Country &rhs) = default;
  Country &operator=(Country &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Country";
private:
  std::string mName;
  Person mLeader;
};
}
          )R");
  ExpectGeneratedContent(files["gen/Country.cpp"], FileType::CppSource, "gen/Country.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Country.h"

#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"

namespace generator_test_namespace {
const std::string &Country::GetName() const {
  return mName;
}

std::string &Country::GetName() {
  return mName;
}

const Person &Country::GetLeader() const {
  return mLeader;
}

Person &Country::GetLeader() {
  return mLeader;
}

void Country::SetName(const std::string &val) {
  mName = val;
}

void Country::SetLeader(const Person &val) {
  mLeader = val;
}

bool Country::operator==(const Country &rhs) const {
  return !(
      mName != rhs.mName ||
      mLeader != rhs.mLeader
  );
}

bool Country::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("leader", name)) {
        auto res = JsonHelper::Parse(mLeader, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.leader field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Country: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Country!");
      auto res = JsonHelper::Parse(mLeader, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.leader field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Country!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Country.");
    return false;
  }
  return true;
}

rapidjson::Value Country::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("leader", JsonHelper::Dump(mLeader, doc), doc.GetAllocator());
  return val;
}
}
          )R");
}

TEST(GeneratorTest, DataManager) {
  Tokenizer tokenizer(R"R(
  @managed(by=Country, field=people)
  @no(json)
  @no(lua)
  struct Person {
    @id
    u32 id;
    string name;
    Ref<Person> partner;
  }
  @dataManager
  struct Country {
    @container(elemName=person)
    vector<Person> people;
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));

  ExpectGeneratedContent(files["gen/Person.h"], FileType::CppHeader, "gen/Person.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>

namespace generator_test_namespace {
// Defined in GeneratorTest
class Person {
public:
  Person() = default;
  Person(const Person &rhs) = default;
  Person(Person &&rhs) noexcept = default;
  ~Person() = default;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  uint32_t GetPartnerId() const;
  const Person *GetPartner() const;
  Person *GetPartner();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  void SetPartnerId(uint32_t val);
  static Person *Get(uint32_t id);
  bool operator==(const Person &rhs) const;
  Person &operator=(const Person &rhs) = default;
  Person &operator=(Person &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Person";
private:
  uint32_t mId = -1;
  std::string mName;
  uint32_t mPartnerId = -1;
};
}
          )R");

  ExpectGeneratedContent(files["gen/Person.cpp"], FileType::CppSource, "gen/Person.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include "Country.h"
#include "GlobalPointer.h"

namespace generator_test_namespace {
uint32_t Person::GetId() const {
  return mId;
}

const std::string &Person::GetName() const {
  return mName;
}

std::string &Person::GetName() {
  return mName;
}

uint32_t Person::GetPartnerId() const {
  return mPartnerId;
}

const Person *Person::GetPartner() const {
  return Person::Get(mPartnerId);
}

Person *Person::GetPartner() {
  return Person::Get(mPartnerId);
}

void Person::SetId(uint32_t val) {
  mId = val;
}

void Person::SetName(const std::string &val) {
  mName = val;
}

void Person::SetPartnerId(uint32_t val) {
  mPartnerId = val;
}

Person *Person::Get(uint32_t id) {
  return GlobalPointer<Country>::GetInstance()->GetPerson(id);
}

bool Person::operator==(const Person &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mPartnerId != rhs.mPartnerId
  );
}
}
          )R");
}

TEST(GeneratorTest, NonCopyableStruct) {
  Tokenizer tokenizer(R"R(
  @struct(nonCopyable)
  @no(json)
  @no(lua)
  struct Person {
  }
    )R",
                      "GeneratorTest");
  ProjectDefinition projectDefinition;
  Parser{projectDefinition, tokenizer}.Parse();
  TranslatorSettings translatorSettings{
      .mNamespace = "generator_test_namespace",
  };
  translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  auto translatedProject = Translator(translatorSettings).Translate(projectDefinition);
  GeneratorSettings generatorSettings{
      .mProjectName = "generator_test_cmake",
      .mConfigHeader = "",
      .mCSharpRoot = "",
  };
  generatorSettings.EnableFeature(GeneratorFeatureFlag::Lua);
  CodeGenerator generator(generatorSettings);
  auto files = MapByName(generator.Generate(translatedProject));

  ExpectGeneratedContent(files["gen/Person.h"], FileType::CppHeader, "gen/Person.h",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
namespace generator_test_namespace {
// Defined in GeneratorTest
class Person {
public:
  Person() = default;
  Person(const Person &rhs) = delete;
  Person(Person &&rhs) noexcept;
  ~Person() = default;
  bool operator==(const Person &rhs) const;
  Person &operator=(const Person &rhs) = delete;
  Person &operator=(Person &&rhs) noexcept;
  inline static const char *CLASS_NAME = "Person";
};
}
        )R");

  ExpectGeneratedContent(files["gen/Person.cpp"], FileType::CppSource, "gen/Person.cpp",
                         R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

namespace generator_test_namespace {
Person::Person(Person &&rhs) noexcept {
}

bool Person::operator==(const Person &rhs) const {
  return true;
}

Person &Person::operator=(Person &&rhs) noexcept {
  return *this;
}
}
        )R");
}


} // namespace
