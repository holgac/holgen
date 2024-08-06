enum TestEnum {
    Entry5 = 5;
    Entry1;
    Entry2;
}

enum TestEnumDefaultValue {
    SomeEntry;
    @enum(default)
    DefaultEntry;
    AnotherEntry;
}

struct TestEnumStruct {
    TestEnum enumField;
    TestEnumDefaultValue enumDefaultValueField;
}

@enumProperty(name=plural, type=string, default=NOT_DEFINED)
@enumProperty(name=maxAge, type=u16, default=20)
enum TestEnumProperty {
    @enumProperty(name=maxAge, value=80)
    Human;
    @enumProperty(name=plural, value=Elves)
    @enumProperty(name=maxAge, value=200)
    Elf;
    @enumProperty(name=plural, value=Goblins)
    Goblin;
}
