struct TestContainerInnerStructWithId {
    @id
    u32 id;
    string name;
}

struct  TestContainerInnerStructNoId {
    u32 field;
    string name;
}

struct TestContainerVector {
    @container(elemName=innerStructWithId)
    @index(on=name)
    vector<TestContainerInnerStructWithId> innerStructsWithId;

    @container(elemName=innerStructNoId)
    vector<TestContainerInnerStructNoId> innerStructsNoId;

    @container(elemName=stringElem)
    vector<string> stringContainer;

    @container(elemName=unsignedElem)
    vector<u32> unsignedContainer;
}

struct TestContainerMap {
    @container(elemName=innerStructWithId)
    @index(on=name)
    map<Ref<TestContainerInnerStructWithId>, TestContainerInnerStructWithId> innerStructsWithId;
}
