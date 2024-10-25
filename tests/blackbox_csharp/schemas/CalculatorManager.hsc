@singleton
struct CalculatorManager {
    func NewCalculator(string name, ICalculator calculator);
    func GetCalculator(string name) -> ICalculator nullable;
    vector<unique_ptr<ICalculator>> calculators;
}
