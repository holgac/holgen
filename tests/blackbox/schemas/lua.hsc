struct Number {
    s64 value;
}

struct Calculator {
    Number curVal;

    @luaFunc
    func add(s64 val) -> s64;

    @luaFunc(table=Ops)
    func subtract(Number val) -> Number;
}