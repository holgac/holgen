@variant(enum=TestVariantStructType, entry=Human)
struct TestVariantStructHuman {
  string name = '"Jean"';
  string nationality = '"Mixed"';
}

@variant(enum=TestVariantStructType, entry=Cat)
struct TestVariantStructCat {
  string name = '"Whiskers"';
  string color = '"orange"';
}

enum TestVariantStructType {
  Human;
  Cat;
}

struct TestVariantStructDifferentTypes {
  @variant(enum=TestVariantStructType, typeField=being1Type)
  variant being1;
  @variant(enum=TestVariantStructType, typeField=being2Type)
  variant being2;
}

struct TestVariantStructSharedType {
  @variant(enum=TestVariantStructType, typeField=beingType)
  variant being1;
  @variant(enum=TestVariantStructType, typeField=beingType)
  variant being2;
}

struct TestVariantStructExplicitType {
  TestVariantStructType type;

  @variant(typeField=type)
  variant being1;
  @variant(typeField=type)
  variant being2;
}
