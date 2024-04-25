@variant(enum=TestVariantStructType, entry=Type1)
struct TestVariantStructCustomData1 {
  u32 f1 = 43;
  float f2 = 44;
  u64 f3 = 45;
}

@variant(enum=TestVariantStructType, entry=Type2)
struct TestVariantStructCustomData2 {
  u64 f1 = 46;
  float f2 = 47;
  u32 f3 = 48;
}

enum TestVariantStructType {
  Type1;
  Type2;
}

struct TestVariantStruct {
  @variant(enum=TestVariantStructType, typeField=customDataType)
  variant customData;
}