@script(alwaysMirror)
struct ModuleVersion {
    u16 major;
    u16 minor;
    u32 revision;
    @func(constructor)
    func Construct(u16 major, u16 minor, u32 revision);
    @func(static)
    func ConstructDouble(u16 major, u16 minor, u32 revision) -> ModuleVersion;
    @func(const)
    func GetSum() -> u32;
    func BumpRevision();
    func SumOfSelfAnd(ModuleVersion v1, ModuleVersion v2) -> ModuleVersion;
    func GetNextRevisions(u64 count) -> vector<ModuleVersion>;
    func GetNextThreeRevisions() -> ModuleVersion[3];
    func GetDataAsVector() -> vector<u32>;
    func GetDataAsArray() -> u32[3];
    func GetDataAsSignedVector() -> vector<s32>;
    func GetEnums(vector<EnumTest> value) -> vector<EnumTest>;
}

@dotNetModule
struct DotNetModule {
    func Initialize();
    func BumpAndGetSingletonCounter() -> u32;
    func SetBumpAmount(u32 amount);
    func Constructor() -> ModuleVersion;
    func MirroredStructManagedToNative() -> ModuleVersion;
    func MirroredStructNativeToManagedToNative() -> ModuleVersion;
    func MirroredStructMethodCall() -> ModuleVersion;
    func MirroredStructInArguments() -> ModuleVersion;
    func SetCounterName(string name);
    func ProxyObjectNativeToManaged() -> u32;
    func ProxyObjectMethodArgInModule(Counter counter) -> u32;
    func ProxyObjectReturnValue(Counter counter1, Counter counter2) -> Counter ref;
    func ProxyObjectField();
    func PrimitiveArrays() -> u32;
    func StringArraysNativeToManaged(vector<string> args);
    func StringArraysManagedToNative();
    func MirroredStructReturnVector() -> u32;
    func MirroredStructReturnArray() -> u32;
    func PrimitiveReturnVector() -> s32;
    func PrimitiveReturnArray() -> u32;
    func TrackedCSharpObject(u32 addCount);
    func EnumArgumentAndReturnValue(EnumTest value) -> EnumTest;
    func EnumVectorArgumentAndReturnValue() -> string;
}
