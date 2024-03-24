struct Number {
    s64 value;
}

struct Calculator {
    Number curVal;

    @luaFunc
    func Add(s64 val) -> s64;

    @luaFunc(table=Ops)
    func Subtract(Number val) -> Number;

    @cppFunc
    func SubtractThenMultiply(s64 lhs, s64 rhs) -> s64;
}