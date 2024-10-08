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
    @index(on=name)
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

struct TestContainerSet {
    @container(elemName=stringElem)
    set<string> stringContainer;

    @container(elemName=unsignedElem)
    set<u32> unsignedContainer;
}

enum TestContainerSkill {
    Strength;
    Perception;
    Endurance;
    Charisma;
}

struct TestContainerEnum {
    @container(elemName=skill)
    u8[TestContainerSkill] skills;
}