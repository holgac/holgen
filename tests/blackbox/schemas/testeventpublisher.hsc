@no(json)
@luaFuncTable
struct TestEventListener {
    string name;
    func OnEvent1(int val);
    func OnEvent2() -> int;
}

@no(json)
@luaFuncTable(static, publisher)
struct TestStaticEventPublisher : TestEventListener {
}
