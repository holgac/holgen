@singleton
struct SingletonCounter {
    @func(const)
    func Get() -> u32;
    func Bump(u32 amount);
    func Reset();
    u32 val;
}