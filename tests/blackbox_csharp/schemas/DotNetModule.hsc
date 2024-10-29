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
    func PrimitiveArrays() -> u32;
    func StringArraysNativeToManaged(vector<string> args);
    func StringArraysManagedToNative();
}
