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
