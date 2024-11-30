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
    @id
    s32 id;
    @compositeId(version)
    s16 version;
}

@compositeId(type=TestCompositeIdCompositeId, entry=Human)
struct TestCompositeIdHuman : TestCompositeIdObject {
    string name;
}

@compositeId(type=TestCompositeIdCompositeId, entry=Cat)
struct TestCompositeIdCat : TestCompositeIdObject {
    string name;
    string breed;
}

@compositeId(type=TestCompositeIdCompositeId, entry=Dog)
struct TestCompositeIdDog : TestCompositeIdObject {
    string breed;
}

struct TestCompositeIdContainer {
    @container(elemName=human)
    @index(on=name)
    deque<TestCompositeIdHuman> humans;
    //    @container(elemName=cat)
    //    @index(on=name)
    //    deque<TestCompositeIdCat> cats;
    //    @container(elemName=dog)
    //    deque<TestCompositeIdDog> dogs;
}