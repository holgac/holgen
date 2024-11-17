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
  @no(json)
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
  @no(lua)
  @no(json)
  u8[16] customData;
  @no(lua)
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
    @func(hash)
    func Hash() -> u64;
}

struct TestStructHashableMap {
    unordered_map<TestStructHashable, string> data;
}

struct TestStructComparable {
    u32 field1;
    u32 field2;
    @func(const)
    @no(lua)
    func operator<(TestStructComparable rhs) -> bool;
}

struct TestStructComparableMap {
    map<TestStructComparable, string> data;
}

struct TestStructPairFields {
    pair<s32, string> intStringPair;
    vector<pair<string, s32>> pairVector;
}

struct TestStructToString {
    string field1;
    string field2;
    @func(toString)
    func Stringify() -> string;
}

struct TestStructConstructor {
    float x;
    float y;
    float z;
    @func(constructor)
    func Construct1(float val) -> string;
    @func(constructor)
    func Construct3(float x, float y, float z) -> string;
}

struct TestStructVirtualMethods {
    @func(virtual)
    func VirtualFunc() -> u32;
    @func(pureVirtual)
    func PureVirtualFunc() -> u32;
}

struct TestStructSmartPointers {
    @no(json)
    shared_ptr<TestStructVirtualMethods> sharedPtr;
    @no(json)
    vector<shared_ptr<TestStructVirtualMethods>> sharedPtrs;
    @no(json)
    unique_ptr<TestStructVirtualMethods> uniquePtr;
    @no(json)
    vector<unique_ptr<TestStructVirtualMethods>> uniquePtrs;
}
