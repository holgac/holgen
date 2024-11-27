enum TestCompositeIdObjectType {
    Human;
    Cat;
    Dog;
}

@compositeIdType
struct TestCompositeIdCompositeId {
    @compositeIdType(type)
    TestCompositeIdObjectType type;
    @compositeIdType(version)
    s16 version;
    @compositeIdType(id)
    s32 id;
}