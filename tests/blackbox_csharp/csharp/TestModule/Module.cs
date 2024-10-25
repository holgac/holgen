using System.Runtime.InteropServices;

namespace TestModule;

public class Module
{
    public static void Main()
    {
        Console.WriteLine("This should never be called; it's for the build system to generate a runtimeconfig.json");
    }

    [UnmanagedCallersOnly]
    public static int Initialize()
    {
        Console.WriteLine("Hello from TestModule!");
        return 0;
    }
}