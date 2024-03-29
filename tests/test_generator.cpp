#include <gtest/gtest.h>
#include "generator/CodeGenerator.h"
#include "Helpers.h"

using namespace holgen;
using namespace holgen::helpers;

namespace {

  TEST(GeneratorTest, Helpers) {
    Tokenizer tokenizer(R"DELIM(
  @noLua()
  struct Person   {
    @jsonConvert(from=string, using=stringToU32Converter)
    u32 age;
    float gender;
  }
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            "add_library(generator_test_cmake Person.cpp JsonHelper.cpp Converter.cpp LuaHelper.cpp GlobalPointer.cpp FilesystemHelper.cpp)"
        }
    );
    // TODO: test helpers
  }

  TEST(GeneratorTest, ClassWithGetters) {
    Tokenizer tokenizer(R"DELIM(
  @noLua()
  @noJson()
  struct Person   {
    u32 age = 0;
    float gender;
  }
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            R"DELIM(
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
protected:
private:
  uint32_t mAge = 0;
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
          )DELIM"
        }
    );
  }

  TEST(GeneratorTest, ClassWithContainers) {
    Tokenizer tokenizer(R"DELIM(
  @noLua()
  struct Market {
    vector<string> instruments;
    map<string, double> prices;
  }
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            R"DELIM(
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
  @noLua()
  struct Sound {
    string name;
    u32 volume;
  }
  @noLua()
  struct Animal {
    vector<Sound> sounds;
  }
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            R"DELIM(
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
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            R"DELIM(
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
protected:
private:
  std::string mName;
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

  TEST(GeneratorTest, DataManager) {
    Tokenizer tokenizer(R"DELIM(
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
    )DELIM", "GeneratorTest");
    Parser parser;
    parser.Parse(tokenizer);
    auto translatedProject = Translator(parser.GetProject()).Translate();
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
            R"DELIM(
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
protected:
private:
  uint32_t mId;
  std::string mName;
  uint32_t mPartnerId = -1;
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
          )DELIM"
        }
    );
  }

}
