@luaFuncTable
struct TestLuaFuncTable {
    func SetField(TestLuaFuncTableContainer container);
    func GetField(TestLuaFuncTableContainer container) -> s32;
}

@luaFuncTable(sourceTable=Scripts)
struct TestLuaFuncTableWithSourceTable {
    func SetField(TestLuaFuncTableContainer container);
    func GetField(TestLuaFuncTableContainer container) -> s32;
}

@luaFuncTable(static)
struct TestLuaFuncTableStatic {
    func SetField(TestLuaFuncTableContainer container);
    func GetField(TestLuaFuncTableContainer container) -> s32;
}

struct TestLuaFuncTableContainer {
    s32 field;
    TestLuaFuncTable script1;
    TestLuaFuncTable script2;
    TestLuaFuncTableWithSourceTable scriptWithSourceTable1;
    TestLuaFuncTableWithSourceTable scriptWithSourceTable2;
    TestLuaFuncTableStatic staticScript1;
    TestLuaFuncTableStatic staticScript2;
}

struct TestLuaNumber {
    s32 value = 0;
}

struct TestLuaCalculator {
    TestLuaNumber lastValue;

    @luaFunc
    func AddPrimitive(s32 num) -> s32;
    @luaFunc
    func AddRef(TestLuaNumber num ref) -> s32;
    @luaFunc
    func AddNullable(TestLuaNumber num nullable) -> s32;

    @luaFunc
    func ReturnNullable(s32 num) -> TestLuaNumber nullable;
    @luaFunc
    func ReturnRef(s32 num) -> TestLuaNumber ref;
    // TODO: implement reading mirror objects
    @luaFunc
    func ReturnNew(s32 num) -> TestLuaNumber new;

/*
    func AddPrimitiveReturnPrimitiveFromCpp(s32 num) -> s32;
    func AddPrimitiveReturnRefFromCpp(s32 num) -> TestLuaNumber ref;
    func AddPrimitiveReturnNullableFromCpp(s32 num) -> TestLuaNumber nullable;
    func AddPrimitiveReturnNewFromCpp(s32 num) -> TestLuaNumber new;
    func AddRefReturnNewFromCpp(TestLuaNumber num ref) -> TestLuaNumber new;
    func AddNullableReturnNewFromCpp(TestLuaNumber num nullable) -> TestLuaNumber nullable;
    func ReturnNullFromCpp() -> TestLuaNumber nullable;
*/
}

@luaFuncTable
struct TestLuaRegistryData {
    luadata data;
    func Init(luadata initData);
    func Get() -> s32;
    func Add(s32 val);
}

struct TestLuaStaticCppFunction {
    u32 data;
    @func(static)
    func Factory(u32 data) -> TestLuaStaticCppFunction;
}