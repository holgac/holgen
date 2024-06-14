struct Number {
    s64 value;
}

struct Calculator {
    Number curVal;

    @luaFunc
    func Add(s64 val) -> s64;

    @luaFunc(sourceTable=Ops)
    func Subtract(Number val) -> Number;

    func SubtractThenMultiply(s64 lhs, s64 rhs) -> s64;
}

struct LuaTestStructContainer {
  @container(elemName=testVectorElem)
  vector<u32> testVector;

  map<string, u32> testMap;
}