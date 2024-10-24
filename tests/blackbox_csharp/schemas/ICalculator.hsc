@interface
struct ICalculator {
    func Set(double val);
    func Add(double val);
    func Subtract(double val);
    func Multiply(double val);
    func Divide(double val);
    @func(const)
    func Get() -> double;
}

