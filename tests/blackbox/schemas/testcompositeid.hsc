enum TestCompositeIdObjectType {
    Human;
    Cat;
    Dog;
}

@compositeIdType(getter=GetCompositeId)
struct TestCompositeIdCompositeId {
    @compositeIdType(type)
    TestCompositeIdObjectType type;
    @compositeIdType(version)
    s16 version;
    @compositeIdType(id)
    s32 id;
}

mixin TestCompositeIdObject {
    @compositeId(id)
    s32 id;
    @compositeId(version)
    s16 version;
}

@compositeId(type=TestCompositeIdCompositeId, entry=Human)
struct TestCompositeIdHuman : TestCompositeIdObject {
}