#include <gtest/gtest.h>
#include "Helpers.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"

using namespace holgen;

class ParserTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}

  ProjectDefinition Parse(const char *text) {
    auto trimmed = helpers::Trim(text);
    Tokenizer tokenizer(trimmed, Source);
    ProjectDefinition projectDefinition;
    Parser{projectDefinition, tokenizer}.Parse();
    return projectDefinition;
  }

  template <typename T>
  void ExpectDefinitionSource(const T &data, size_t line, size_t col) {
    auto &definiitonSource = data.mDefinitionSource;
    EXPECT_EQ(definiitonSource.mSource, Source);
    EXPECT_EQ(definiitonSource.mLine, line);
    EXPECT_EQ(definiitonSource.mColumn, col);
  }

  void ExpectAnnotationAttribute(const AnnotationAttributeDefinition &annotationAttribute,
                                 const std::string &name, const std::string &value, size_t line,
                                 size_t col) {
    EXPECT_EQ(annotationAttribute.mName, name);
    EXPECT_EQ(annotationAttribute.mValue.mName, value);
    ExpectDefinitionSource(annotationAttribute, line, col);
  }

  void ExpectAnnotation(const AnnotationDefinition &annotation, const std::string &name,
                        size_t attributeCount, size_t line, size_t col) {
    EXPECT_EQ(annotation.mName, name);
    EXPECT_EQ(annotation.mAttributes.size(), attributeCount);
    ExpectDefinitionSource(annotation, line, col);
  }

  void ExpectField(const FieldDefinition &field, size_t line, size_t col, const std::string &name,
                   const std::string &type, size_t annotationCount,
                   const std::optional<std::string> &defaultValue, const std::string &arraySize) {
    EXPECT_EQ(field.mName, name);
    EXPECT_EQ(field.mType.mName, type) << " in field " << name;
    EXPECT_EQ(field.mType.mArraySize, arraySize) << " in field " << name;
    EXPECT_EQ(field.mAnnotations.size(), annotationCount) << " in field " << name;
    EXPECT_EQ(field.mDefaultValue, defaultValue) << " in field " << name;
    ExpectDefinitionSource(field, line, col);
  }

  void ExpectStruct(const StructDefinition &s, const std::string &name, size_t fieldCount,
                    size_t annotationCount, size_t line, size_t col) {
    EXPECT_EQ(s.mName, name);
    EXPECT_EQ(s.mFields.size(), fieldCount);
    EXPECT_EQ(s.mAnnotations.size(), annotationCount);
    ExpectDefinitionSource(s, line, col);
  }

  void ExpectEnum(const EnumDefinition &e, const std::string &name, const std::string &invalidValue,
                  size_t entryCount, size_t annotationCount, size_t line, size_t col) {
    EXPECT_EQ(e.mName, name);
    EXPECT_EQ(e.mInvalidValue, invalidValue);
    EXPECT_EQ(e.mEntries.size(), entryCount);
    EXPECT_EQ(e.mAnnotations.size(), annotationCount);
    ExpectDefinitionSource(e, line, col);
  }

  void ExpectEnumEntry(const EnumEntryDefinition &e, const std::string &name,
                       const std::string &value, size_t annotationCount, size_t line, size_t col) {
    EXPECT_EQ(e.mName, name);
    EXPECT_EQ(e.mValue, value);
    EXPECT_EQ(e.mAnnotations.size(), annotationCount);
    ExpectDefinitionSource(e, line, col);
  }

  void ExpectFunction(const FunctionDefinition &functionDefinition, const std::string &name,
                      const FunctionReturnTypeDefinition &returnType, size_t argCount,
                      size_t annotationCount, size_t line, size_t col) {
    EXPECT_EQ(functionDefinition.mName, name);
    helpers::ExpectEqual(functionDefinition.mReturnType, returnType);
    EXPECT_EQ(functionDefinition.mArguments.size(), argCount);
    EXPECT_EQ(functionDefinition.mAnnotations.size(), annotationCount);
    ExpectDefinitionSource(functionDefinition, line, col);
  }

  void ExpectFunctionArgument(const FunctionArgumentDefinition &arg, const std::string &name,
                              const std::string &type, Nullability nullability, Constness constness,
                              size_t line, size_t col) {
    EXPECT_EQ(arg.mName, name);
    EXPECT_EQ(arg.mType.mName, type);
    EXPECT_EQ(arg.mNullability, nullability);
    EXPECT_EQ(arg.mConstness, constness);
    ExpectDefinitionSource(arg, line, col);
  }

  FunctionReturnTypeDefinition ReturnType(const std::string &type) {
    FunctionReturnTypeDefinition frt;
    frt.mType.mName = type;
    return frt;
  }

  const char *Source = "ParserTest";
};

TEST_F(ParserTest, EmptyStruct) {
  auto proj = Parse("struct a   { }");
  EXPECT_EQ(proj.mStructs.size(), 1);
  ExpectStruct(proj.mStructs[0], "a", 0, 0, 0, 0);
}

TEST_F(ParserTest, StructFields) {
  auto proj = Parse(R"R(
struct a   {
  s32 f1;
  float f2;
  double[5] arr;
  })R");
  EXPECT_EQ(proj.mStructs.size(), 1);
  ExpectStruct(proj.mStructs[0], "a", 3, 0, 0, 0);
  ExpectField(proj.mStructs[0].mFields[0], 1, 2, "f1", "s32", 0, std::nullopt, "");
  ExpectField(proj.mStructs[0].mFields[1], 2, 2, "f2", "float", 0, std::nullopt, "");
  ExpectField(proj.mStructs[0].mFields[2], 3, 2, "arr", "double", 0, std::nullopt, "5");
}

TEST_F(ParserTest, EnumSizedArray) {
  auto proj = Parse(R"R(
enum Enum {
  Entry0;
  Entry1;
}
struct a   {
  double[Enum] arr;
  })R");
  EXPECT_EQ(proj.mStructs.size(), 1);
  ExpectStruct(proj.mStructs[0], "a", 1, 0, 4, 0);
  ExpectField(proj.mStructs[0].mFields[0], 5, 2, "arr", "double", 0, std::nullopt, "Enum");
}

TEST_F(ParserTest, StructFieldDefaultValues) {
  auto proj = Parse(R"R(
struct a   {
  float one = 1;
  float onePoint = 1.;
  float pointOne = .1;
  float onePointOne = 1.1;
  })R");
  EXPECT_EQ(proj.mStructs.size(), 1);
  ExpectStruct(proj.mStructs[0], "a", 4, 0, 0, 0);
  ExpectField(proj.mStructs[0].mFields[0], 1, 2, "one", "float", 0, "1", "");
  ExpectField(proj.mStructs[0].mFields[1], 2, 2, "onePoint", "float", 0, "1.", "");
  ExpectField(proj.mStructs[0].mFields[2], 3, 2, "pointOne", "float", 0, ".1", "");
  ExpectField(proj.mStructs[0].mFields[3], 4, 2, "onePointOne", "float", 0, "1.1", "");
}

TEST_F(ParserTest, FieldAnnotations) {
  auto proj = Parse(R"R(
struct a   {
  @dec1()
  @dec2(a1, a2=5,
    a3, a4='long string')
  s32 f1;
@dec3(a)
  u32 f2;
})R");
  EXPECT_EQ(proj.mStructs.size(), 1);
  auto &s = proj.mStructs[0];
  ExpectStruct(s, "a", 2, 0, 0, 0);
  ExpectField(s.mFields[0], 4, 2, "f1", "s32", 2, std::nullopt, "");
  ExpectAnnotation(s.mFields[0].mAnnotations[0], "dec1", 0, 1, 2);
  ExpectAnnotation(s.mFields[0].mAnnotations[1], "dec2", 4, 2, 2);
  ExpectAnnotationAttribute(s.mFields[0].mAnnotations[1].mAttributes[0], "a1", "", 2, 8);
  ExpectAnnotationAttribute(s.mFields[0].mAnnotations[1].mAttributes[1], "a2", "5", 2, 12);
  ExpectAnnotationAttribute(s.mFields[0].mAnnotations[1].mAttributes[2], "a3", "", 3, 4);
  ExpectAnnotationAttribute(s.mFields[0].mAnnotations[1].mAttributes[3], "a4", "long string", 3, 8);
  ExpectField(s.mFields[1], 6, 2, "f2", "u32", 1, std::nullopt, "");
  ExpectAnnotation(s.mFields[1].mAnnotations[0], "dec3", 1, 5, 0);
  ExpectAnnotationAttribute(s.mFields[1].mAnnotations[0].mAttributes[0], "a", "", 5, 6);
}

TEST_F(ParserTest, EmptyAnnotation) {
  auto proj = Parse(R"R(
@dec1
struct a   {
  @dec2
  s32 f1;
})R");
  ASSERT_EQ(proj.mStructs.size(), 1);
  ExpectStruct(proj.mStructs[0], "a", 1, 1, 1, 0);
  ExpectAnnotation(proj.mStructs[0].mAnnotations[0], "dec1", 0, 0, 0);
  ExpectField(proj.mStructs[0].mFields[0], 3, 2, "f1", "s32", 1, std::nullopt, "");
  ExpectAnnotation(proj.mStructs[0].mFields[0].mAnnotations[0], "dec2", 0, 2, 2);
}

TEST_F(ParserTest, StructAnnotations) {
  auto proj = Parse(R"R(
@dec1()
@dec2(a1, a2=5, a3, a4='long string')
struct a   {
  s32 f1;
  float f2;
})R");
  ExpectStruct(proj.mStructs[0], "a", 2, 2, 2, 0);
  ExpectAnnotation(proj.mStructs[0].mAnnotations[0], "dec1", 0, 0, 0);
  ExpectAnnotation(proj.mStructs[0].mAnnotations[1], "dec2", 4, 1, 0);
  ExpectAnnotationAttribute(proj.mStructs[0].mAnnotations[1].mAttributes[0], "a1", "", 1, 6);
  ExpectAnnotationAttribute(proj.mStructs[0].mAnnotations[1].mAttributes[1], "a2", "5", 1, 10);
  ExpectAnnotationAttribute(proj.mStructs[0].mAnnotations[1].mAttributes[2], "a3", "", 1, 16);
  ExpectAnnotationAttribute(proj.mStructs[0].mAnnotations[1].mAttributes[3], "a4", "long string", 1,
                            20);
}

TEST_F(ParserTest, Templates) {
  auto proj = Parse(R"R(
  // comment
  struct a   {
    map<u32, map<string, vector<float>>> myMap;
  })R");
  EXPECT_EQ(proj.mStructs.size(), 1);
  auto &s = proj.mStructs[0];
  ExpectStruct(s, "a", 1, 0, 1, 2);
  ExpectField(s.mFields[0], 2, 4, "myMap", "map", 0, std::nullopt, "");
  auto &t = s.mFields[0].mType;
  EXPECT_EQ(t.mName, "map");
  EXPECT_EQ(t.mTemplateParameters.size(), 2);
  EXPECT_EQ(t.mTemplateParameters[0].mName, "u32");
  EXPECT_EQ(t.mTemplateParameters[0].mTemplateParameters.size(), 0);
  EXPECT_EQ(t.mTemplateParameters[1].mName, "map");
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters.size(), 2);
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[0].mName, "string");
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[0].mTemplateParameters.size(), 0);
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mName, "vector");
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mTemplateParameters.size(), 1);
  EXPECT_EQ(t.mTemplateParameters[1].mTemplateParameters[1].mTemplateParameters[0].mName, "float");
  EXPECT_EQ(t.mTemplateParameters[1]
                .mTemplateParameters[1]
                .mTemplateParameters[0]
                .mTemplateParameters.size(),
            0);
}

TEST_F(ParserTest, NestedStructs) {
  auto proj = Parse(R"R(
struct Sound {
  string name;
  u32 volume;
}
struct Animal {
  Sound sound;
}
    )R");
  EXPECT_EQ(proj.mStructs.size(), 2);
  ExpectStruct(proj.mStructs[0], "Sound", 2, 0, 0, 0);
  ExpectField(proj.mStructs[0].mFields[0], 1, 2, "name", "string", 0, std::nullopt, "");
  ExpectField(proj.mStructs[0].mFields[1], 2, 2, "volume", "u32", 0, std::nullopt, "");
  ExpectStruct(proj.mStructs[1], "Animal", 1, 0, 4, 0);
  ExpectField(proj.mStructs[1].mFields[0], 5, 2, "sound", "Sound", 0, std::nullopt, "");
}

TEST_F(ParserTest, Enums) {
  auto proj = Parse(R"R(
enum LandscapeType {
  Land;
  River = 2;
  Sea;
  Mountain;
})R");
  EXPECT_EQ(proj.mEnums.size(), 1);
  ExpectEnum(proj.mEnums[0], "LandscapeType", "4", 4, 0, 0, 0);
  ExpectEnumEntry(proj.mEnums[0].mEntries[0], "Land", "0", 0, 1, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[1], "River", "2", 0, 2, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[2], "Sea", "1", 0, 3, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[3], "Mountain", "3", 0, 4, 2);
}

TEST_F(ParserTest, EnumNegativeEntry) {
  auto proj = Parse(R"R(
enum LandscapeType {
  Land;
  River = -1;
  Sea;
  Mountain = -2;
})R");
  EXPECT_EQ(proj.mEnums.size(), 1);
  ExpectEnum(proj.mEnums[0], "LandscapeType", "2", 4, 0, 0, 0);
  ExpectEnumEntry(proj.mEnums[0].mEntries[0], "Land", "0", 0, 1, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[1], "River", "-1", 0, 2, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[2], "Sea", "1", 0, 3, 2);
  ExpectEnumEntry(proj.mEnums[0].mEntries[3], "Mountain", "-2", 0, 4, 2);
}

TEST_F(ParserTest, Functions) {
  auto proj = Parse(R"R(
struct Actor {
  string name;
}
struct Action {
  func func1(Actor actor);
  func func2(s32 i1, vector<s32> i2) -> void;
  func func3() -> vector<s32>;
  func func4(u32 arg=42);
}
  )R");
  auto action = proj.GetStruct("Action");
  ASSERT_NE(action, nullptr);
  EXPECT_EQ(action->mFunctions.size(), 4);
  auto func = action->GetFunction("func1");
  ASSERT_NE(func, nullptr);
  ExpectFunction(*func, "func1", ReturnType("void"), 1, 0, 4, 2);
  ExpectFunctionArgument(func->mArguments[0], "actor", "Actor", Nullability::NotNullable,
                         Constness::Const, 4, 13);

  func = action->GetFunction("func2");
  ASSERT_NE(func, nullptr);
  ExpectFunction(*func, "func2", ReturnType("void"), 2, 0, 5, 2);
  ExpectFunctionArgument(func->mArguments[0], "i1", "s32", Nullability::NotNullable,
                         Constness::Const, 5, 13);
  ExpectFunctionArgument(func->mArguments[1], "i2", "vector", Nullability::NotNullable,
                         Constness::Const, 5, 21);
  EXPECT_EQ(func->mArguments[1].mType.mTemplateParameters[0].mName, "s32");

  func = action->GetFunction("func3");
  ASSERT_NE(func, nullptr);
  TypeDefinition s32Vector;
  s32Vector.mName = "vector";
  s32Vector.mTemplateParameters.emplace_back().mName = "s32";
  ExpectFunction(*func, "func3", {s32Vector}, 0, 0, 6, 2);
  EXPECT_EQ(func->mReturnType.mType.mTemplateParameters[0].mName, "s32");

  func = action->GetFunction("func4");
  ASSERT_NE(func, nullptr);
  ExpectFunction(*func, "func4", ReturnType("void"), 1, 0, 7, 2);
  ASSERT_TRUE(func->mArguments.front().mDefaultValue.has_value());
  EXPECT_EQ(*func->mArguments.front().mDefaultValue, "42");
}

TEST_F(ParserTest, FunctionReturnTypeModifiers) {
  auto proj = Parse(R"R(
struct Action {
  func f() -> s32;
  func fc() -> s32 const;
  func fn() -> s32 nullable;
  func fw() -> s32 new;
}
  )R");
  auto action = proj.GetStruct("Action");
  ASSERT_NE(action, nullptr);
  EXPECT_EQ(action->mFunctions.size(), 4);
  ASSERT_NE(action->GetFunction("f"), nullptr);
  TypeDefinition s32;
  s32.mName = "s32";
  ExpectFunction(
      *action->GetFunction("f"), "f",
      FunctionReturnTypeDefinition{s32, Constness::NotConst, FunctionReturnTypeCategory::Reference},
      0, 0, 1, 2);
  ExpectFunction(
      *action->GetFunction("fc"), "fc",
      FunctionReturnTypeDefinition{s32, Constness::Const, FunctionReturnTypeCategory::Reference}, 0,
      0, 2, 2);
  ExpectFunction(
      *action->GetFunction("fn"), "fn",
      FunctionReturnTypeDefinition{s32, Constness::NotConst, FunctionReturnTypeCategory::Pointer},
      0, 0, 3, 2);
  ExpectFunction(
      *action->GetFunction("fw"), "fw",
      FunctionReturnTypeDefinition{s32, Constness::NotConst, FunctionReturnTypeCategory::NewObject},
      0, 0, 4, 2);
}

TEST_F(ParserTest, FunctionArgumentModifiers) {
  auto proj = Parse(R"R(
struct Action {
  func f(s32 i);
  func fr(s32 i ref);
  func fn(s32 i nullable);
  func fnr(s32 i nullable ref);
}
  )R");
  auto action = proj.GetStruct("Action");
  ASSERT_NE(action, nullptr);
  EXPECT_EQ(action->mFunctions.size(), 4);
  ASSERT_NE(action->GetFunction("f"), nullptr);
  ExpectFunction(*action->GetFunction("f"), "f", ReturnType("void"), 1, 0, 1, 2);
  ExpectFunctionArgument(action->GetFunction("f")->mArguments[0], "i", "s32",
                         Nullability::NotNullable, Constness::Const, 1, 9);
  ASSERT_NE(action->GetFunction("fr"), nullptr);
  ExpectFunction(*action->GetFunction("fr"), "fr", ReturnType("void"), 1, 0, 2, 2);
  ExpectFunctionArgument(action->GetFunction("fr")->mArguments[0], "i", "s32",
                         Nullability::NotNullable, Constness::NotConst, 2, 10);
  ASSERT_NE(action->GetFunction("fn"), nullptr);
  ExpectFunction(*action->GetFunction("fn"), "fn", ReturnType("void"), 1, 0, 3, 2);
  ExpectFunctionArgument(action->GetFunction("fn")->mArguments[0], "i", "s32",
                         Nullability::Nullable, Constness::Const, 3, 10);
  ASSERT_NE(action->GetFunction("fnr"), nullptr);
  ExpectFunction(*action->GetFunction("fnr"), "fnr", ReturnType("void"), 1, 0, 4, 2);
  ExpectFunctionArgument(action->GetFunction("fnr")->mArguments[0], "i", "s32",
                         Nullability::Nullable, Constness::NotConst, 4, 11);
}

TEST_F(ParserTest, Mixin) {
  auto proj = Parse("struct A : B { } mixin B {}");
  EXPECT_EQ(proj.mStructs.size(), 2);
  auto a = proj.GetStruct("A");
  ASSERT_NE(a, nullptr);
  EXPECT_EQ(a->mIsMixin, false);
  auto b = proj.GetStruct("B");
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(b->mIsMixin, true);
  ASSERT_EQ(a->mMixins.size(), 1);
  ASSERT_EQ(a->mMixins[0], "B");
}

TEST_F(ParserTest, MultipleMixins) {
  auto proj = Parse("struct A : B, C { } mixin B {} mixin C {}");
  EXPECT_EQ(proj.mStructs.size(), 3);
  auto a = proj.GetStruct("A");
  ASSERT_NE(a, nullptr);
  EXPECT_EQ(a->mIsMixin, false);
  auto b = proj.GetStruct("B");
  ASSERT_NE(b, nullptr);
  EXPECT_EQ(b->mIsMixin, true);
  auto c = proj.GetStruct("C");
  ASSERT_NE(c, nullptr);
  EXPECT_EQ(c->mIsMixin, true);
  ASSERT_EQ(a->mMixins.size(), 2);
  ASSERT_EQ(a->mMixins[0], "B");
  ASSERT_EQ(a->mMixins[1], "C");
}

// TOOD: test fail cases
