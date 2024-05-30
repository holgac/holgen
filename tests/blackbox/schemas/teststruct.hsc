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
  func GetData1() -> TestStructArrayCustomData1;
  func GetData2() -> TestStructArrayCustomData2;
}

@struct(nonCopyable)
struct TestStructNonCopyable {
  vector<int> bigVector;
}