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
