@interface
struct ICounterBumper {
    func SetName(string name);
    func Bump();
    func SetNameFromCounter(Counter ctr);
    func SetNameFromRevision(ModuleVersion version);
    func GetVersion() -> ModuleVersion;
    func GetCounter() -> Counter ref;
    func Clone() -> ICounterBumper;
    func CopyFrom(ICounterBumper other);
    func BumpMultiple(vector<string> names, vector<u32> counts);
    func SetNameConcatArray(string[3] name);
//    func SplitName() -> vector<string>;
//    func BumpWithVector(vector<u32> counts) -> vector<u32>;
//    func BumpWithVector(vector<u32> counts);
//    func GetValue() -> u32;
//    @func(static)
//    func StaticFunction() -> string;
}

struct Counter {
    @func(const)
    func Get() -> u32;
    func Bump(u32 amount);
    func Reset();
    func AddCounterAndReturnSelf(Counter rhs) -> Counter ref;
    func BumpMultiple(span<u32> amounts);
    // Should this really be supported?
    func BumpMultipleCopy(vector<u32> amounts);
    // this is a span but it also incurs a copy when converting from char* to string
    func BumpByStrlensOf(span<string> words);
    func BumpByStrlensOfCopy(vector<string> words);
    u32 val;
}

@singleton
struct CounterManager {
//    func NewCalculator(string name, ICalculator calculator nullable);
//    func GetCalculator(string name) -> ICalculator nullable;
//    vector<unique_ptr<ICalculator>> calculators;
    @no(script)
    map<string, Counter> counters;
    func GetCounter(string name) -> Counter ref;
    func GetCounterPtr(string name) -> Counter nullable;

    @no(script)
    vector<ICounterBumper> counterBumpers;
    func InsertCounterBumper(ICounterBumper bumper new);
}