struct CalculatorManager {
    func RegisterCalculator(ICalculator calculator);
    vector<unique_ptr<ICalculator>> calculators;
}
