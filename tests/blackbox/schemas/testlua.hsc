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

struct TestLuaFuncTableContainer {
    s32 field;
    TestLuaFuncTable script1;
    TestLuaFuncTable script2;
    TestLuaFuncTableWithSourceTable scriptWithSourceTable1;
    TestLuaFuncTableWithSourceTable scriptWithSourceTable2;
}
