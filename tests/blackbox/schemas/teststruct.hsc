struct TestStruct {
    bool testFieldBool;
    u32 testFieldUnsigned = 42;
    string testFieldString;
    userdata testFieldUserdata;
}
struct TestStructSingleElem {
  string name;
}

struct TestStructSingleElemWithId {
  @id
  u32 id;
  string name;
}

struct TestStructSingleElemContainer {
  @container(elemName=singleElemStruct)
  vector<TestStructSingleElem> singleElemStructs;

  @container(elemName=singleElemStructWithId)
  vector<TestStructSingleElemWithId> singleElemStructsWithId;
}

struct TestStructArrayCustomData1 {
  u32 f1 = 43;
  float f2 = 44;
  u64 f3 = 45;
}

struct TestStructArrayCustomData2 {
  u64 f1 = 46;
  float f2 = 47;
  u32 f3 = 48;
}

enum TestStructArrayType {
  Type1;
  Type2;
}

struct TestStructArray {
  // TODO: const
  TestStructArrayType type;
  @noLua
  @noJson
  u8[16] customData;
  @noLua
  func Initialize(TestStructArrayType type);
  func GetData1() -> TestStructArrayCustomData1 nullable;
  func GetData2() -> TestStructArrayCustomData2 nullable;
}

@struct(nonCopyable)
struct TestStructNonCopyable {
  vector<int> bigVector;
}

struct TestStructHashable {
    u32 field1;
    u32 field2;
    @func(const, hash)
    func Hash() -> u64;
}

struct TestStructHashableMap {
    unordered_map<TestStructHashable, string> data;
}

struct TestStructComparable {
    u32 field1;
    u32 field2;
    @func(const)
    @noLua
    func operator<(TestStructComparable rhs) -> bool;
}

struct TestStructComparableMap {
    map<TestStructComparable, string> data;
}

struct TestStructPairFields {
    pair<s32, string> intStringPair;
    vector<pair<string, s32>> pairVector;
}
