#include <gtest/gtest.h>
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "generator/CodeGenerator.h"
#include "Helpers.h"

using namespace holgen;
using namespace holgen::helpers;

namespace {

  TEST(GeneratorTest, Helpers) {
    Tokenizer tokenizer(R"R(
  @noLua()
  struct Person   {
    @jsonConvert(from=string, using=stringToU32Converter)
    u32 age;
    float gender;
  }
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    ExpectGeneratedContent(
        files["CMakeLists.txt"],
        {
            FileType::CMakeFile,
            "CMakeLists.txt",
            R"R(
# This file is generated by holgen. Do not modify manually.
add_library(generator_test_cmake Person.cpp Converter.cpp JsonHelper.cpp LuaHelper.cpp GlobalPointer.cpp FilesystemHelper.cpp)
)R"
        }
    );
    // TODO: test helpers
  }

  TEST(GeneratorTest, ClassWithGetters) {
    Tokenizer tokenizer(R"R(
  @noLua()
  @noJson()
  struct Person   {
    u32 age = 0;
    float gender;
  }
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    ExpectGeneratedContent(
        files["Person.h"],
        {
            FileType::CppHeader,
            "Person.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <cstdint>

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetAge() const;
  float GetGender() const;
  void SetAge(uint32_t val);
  void SetGender(float val);
private:
  uint32_t mAge = 0;
  float mGender;
};
}
)R"
        }
    );
    ExpectGeneratedContent(
        files["Person.cpp"],
        {
            FileType::CppSource,
            "Person.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

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
}
          )R"
        }
    );
  }

  TEST(GeneratorTest, ClassWithContainers) {
    Tokenizer tokenizer(R"R(
  @noLua()
  struct Market {
    vector<string> instruments;
    map<string, double> prices;
  }
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    ExpectGeneratedContent(
        files["Market.h"],
        {
            FileType::CppHeader,
            "Market.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <vector>
#include <string>
#include <map>
#include <rapidjson/fwd.h>
#include "Converter.h"

namespace generator_test_namespace {
class Market {
public:
  const std::vector<std::string>& GetInstruments() const;
  std::vector<std::string>& GetInstruments();
  const std::map<std::string, double>& GetPrices() const;
  std::map<std::string, double>& GetPrices();
  void SetInstruments(const std::vector<std::string>& val);
  void SetPrices(const std::map<std::string, double>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
private:
  std::vector<std::string> mInstruments;
  std::map<std::string, double> mPrices;
};
}
          )R"
        }
    );
    ExpectGeneratedContent(
        files["Market.cpp"],
        {
            FileType::CppSource,
            "Market.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Market.h"

#include <rapidjson/document.h>
#include "JsonHelper.h"

namespace generator_test_namespace {
const std::vector<std::string>& Market::GetInstruments() const {
  return mInstruments;
}

std::vector<std::string>& Market::GetInstruments() {
  return mInstruments;
}

const std::map<std::string, double>& Market::GetPrices() const {
  return mPrices;
}

std::map<std::string, double>& Market::GetPrices() {
  return mPrices;
}

void Market::SetInstruments(const std::vector<std::string>& val) {
  mInstruments = val;
}

void Market::SetPrices(const std::map<std::string, double>& val) {
  mPrices = val;
}

bool Market::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Market");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("instruments", name)) {
      auto res = JsonHelper::Parse(mInstruments, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Market.instruments field");
    } else if (0 == strcmp("prices", name)) {
      auto res = JsonHelper::Parse(mPrices, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Market.prices field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Market: {}", name);
    }
  }
  return true;
}
}
          )R"
        }
    );
  }

  TEST(GeneratorTest, NestedClasses) {
    Tokenizer tokenizer(R"R(
  @noLua()
  struct Sound {
    string name;
    u32 volume;
  }
  @noLua()
  struct Animal {
    vector<Sound> sounds;
  }
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    ExpectGeneratedContent(
        files["Sound.h"],
        {
            FileType::CppHeader,
            "Sound.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

namespace generator_test_namespace {
class Sound {
public:
  const std::string& GetName() const;
  std::string& GetName();
  uint32_t GetVolume() const;
  void SetName(const std::string& val);
  void SetVolume(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
private:
  std::string mName;
  uint32_t mVolume;
};
}
          )R"
        }
    );

    ExpectGeneratedContent(
        files["Sound.cpp"],
        {
            FileType::CppSource,
            "Sound.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Sound.h"

#include <rapidjson/document.h>
#include "JsonHelper.h"

namespace generator_test_namespace {
const std::string& Sound::GetName() const {
  return mName;
}

std::string& Sound::GetName() {
  return mName;
}

uint32_t Sound::GetVolume() const {
  return mVolume;
}

void Sound::SetName(const std::string& val) {
  mName = val;
}

void Sound::SetVolume(uint32_t val) {
  mVolume = val;
}

bool Sound::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Sound");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("name", name)) {
      auto res = JsonHelper::Parse(mName, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Sound.name field");
    } else if (0 == strcmp("volume", name)) {
      auto res = JsonHelper::Parse(mVolume, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Sound.volume field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Sound: {}", name);
    }
  }
  return true;
}
}
          )R"
        }
    );


    ExpectGeneratedContent(
        files["Animal.h"],
        {
            FileType::CppHeader,
            "Animal.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <vector>
#include <rapidjson/fwd.h>
#include "Sound.h"
#include "Converter.h"

namespace generator_test_namespace {
class Animal {
public:
  const std::vector<Sound>& GetSounds() const;
  std::vector<Sound>& GetSounds();
  void SetSounds(const std::vector<Sound>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
private:
  std::vector<Sound> mSounds;
};
}
          )R"
        }
    );

    ExpectGeneratedContent(
        files["Animal.cpp"],
        {
            FileType::CppSource,
            "Animal.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Animal.h"

#include <rapidjson/document.h>
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
  if (json.IsObject()) {
    for(const auto& data: json.GetObject()) {
      const auto& name = data.name.GetString();
      if (0 == strcmp("sounds", name)) {
        auto res = JsonHelper::Parse(mSounds, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Animal.sounds field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Animal: {}", name);
      }
    }
  } else {
    auto res = JsonHelper::Parse(mSounds, json, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Animal.sounds field");
  }
  return true;
}
}
          )R"
        }
    );

  }

  TEST(GeneratorTest, Converters) {
    Tokenizer tokenizer(R"R(
  @noLua()
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
  @noLua()
  struct Country {
    @noJson()
    string name;
    Person leader;
  }
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));
    ExpectGeneratedContent(
        files["Person.h"],
        {
            FileType::CppHeader,
            "Person.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetCurrentCountry() const;
  uint32_t GetCurrentCity() const;
  uint32_t GetHomeCountry() const;
  uint32_t GetPlaceOfBirth() const;
  void SetCurrentCountry(uint32_t val);
  void SetCurrentCity(uint32_t val);
  void SetHomeCountry(uint32_t val);
  void SetPlaceOfBirth(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
private:
  uint32_t mCurrentCountry;
  uint32_t mCurrentCity;
  uint32_t mHomeCountry;
  uint32_t mPlaceOfBirth;
};
}
          )R"
        }
    );
    ExpectGeneratedContent(
        files["Person.cpp"],
        {
            FileType::CppSource,
            "Person.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include <rapidjson/document.h>
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

bool Person::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Person");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
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
  return true;
}
}
          )R"
        }
    );
    ExpectGeneratedContent(
        files["Country.h"],
        {
            FileType::CppHeader,
            "Country.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <string>
#include <rapidjson/fwd.h>
#include "Person.h"
#include "Converter.h"

namespace generator_test_namespace {
class Country {
public:
  const std::string& GetName() const;
  std::string& GetName();
  const Person& GetLeader() const;
  Person& GetLeader();
  void SetName(const std::string& val);
  void SetLeader(const Person& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
private:
  std::string mName;
  Person mLeader;
};
}
          )R"
        }
    );
    ExpectGeneratedContent(
        files["Country.cpp"],
        {
            FileType::CppSource,
            "Country.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Country.h"

#include <rapidjson/document.h>
#include "JsonHelper.h"

namespace generator_test_namespace {
const std::string& Country::GetName() const {
  return mName;
}

std::string& Country::GetName() {
  return mName;
}

const Person& Country::GetLeader() const {
  return mLeader;
}

Person& Country::GetLeader() {
  return mLeader;
}

void Country::SetName(const std::string& val) {
  mName = val;
}

void Country::SetLeader(const Person& val) {
  mLeader = val;
}

bool Country::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing Country");
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("leader", name)) {
      auto res = JsonHelper::Parse(mLeader, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Country.leader field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing Country: {}", name);
    }
  }
  return true;
}
}
          )R"
        }
    );
  }

  TEST(GeneratorTest, DataManager) {
    Tokenizer tokenizer(R"R(
  @managed(by=Country, field=people)
  @noJson
  @noLua
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
    )R", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    TranslatorSettings translatorSettings{
        .mNamespace = "generator_test_namespace",
    };
    auto translatedProject = Translator(translatorSettings).Translate(parser.GetProject());
    GeneratorSettings generatorSettings{
        .mNamespace = "generator_test_namespace",
        .mCMakeTarget = "generator_test_cmake",
        .mConfigHeader = "",
    };
    CodeGenerator generator(generatorSettings);
    auto files = MapByName(generator.Generate(translatedProject));

    ExpectGeneratedContent(
        files["Person.h"],
        {
            FileType::CppHeader,
            "Person.h",
            R"R(
// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>

namespace generator_test_namespace {
class Person {
public:
  uint32_t GetId() const;
  const std::string& GetName() const;
  std::string& GetName();
  uint32_t GetPartnerId() const;
  const Person* GetPartner() const;
  Person* GetPartner();
  void SetId(uint32_t val);
  void SetName(const std::string& val);
  void SetPartnerId(uint32_t val);
  static Person* Get(uint32_t id);
private:
  uint32_t mId = -1;
  std::string mName;
  uint32_t mPartnerId = -1;
};
}
          )R"
        }
    );

    ExpectGeneratedContent(
        files["Person.cpp"],
        {
            FileType::CppSource,
            "Person.cpp",
            R"R(
// This file is generated by holgen. Do not modify manually.
#include "Person.h"

#include "GlobalPointer.h"
#include "Country.h"

namespace generator_test_namespace {
uint32_t Person::GetId() const {
  return mId;
}

const std::string& Person::GetName() const {
  return mName;
}

std::string& Person::GetName() {
  return mName;
}

uint32_t Person::GetPartnerId() const {
  return mPartnerId;
}

const Person* Person::GetPartner() const {
  return Person::Get(mPartnerId);
}

Person* Person::GetPartner() {
  return Person::Get(mPartnerId);
}

void Person::SetId(uint32_t val) {
  mId = val;
}

void Person::SetName(const std::string& val) {
  mName = val;
}

void Person::SetPartnerId(uint32_t val) {
  mPartnerId = val;
}

Person* Person::Get(uint32_t id) {
  return GlobalPointer<Country>::GetInstance()->GetPerson(id);
}
}
          )R"
        }
    );
  }

}
