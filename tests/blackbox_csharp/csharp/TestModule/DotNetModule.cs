using System.Runtime.InteropServices;
using CSharpBindings;

namespace TestModule;

public sealed class DotNetModule : IDotNetModule
{
    private string _counterName = "TestModule";
    private static DotNetModule _instance = null!;

    public static void Initialize()
    {
        Console.WriteLine("Hello from TestModule!");
        _instance = new DotNetModule();
    }

    public static ModuleVersion.Fields Constructor()
    {
        return new ModuleVersion(0, 10, 20).Data;
    }

    public static ModuleVersion.Fields MirroredStructManagedToNative()
    {
        ModuleVersion.Fields a;
        a.Major = 0;
        a.Minor = 1;
        a.Revision = 2;
        return a;
    }

    public static ModuleVersion.Fields MirroredStructNativeToManagedToNative()
    {
        return ModuleVersion.ConstructDouble(0, 1, 2).Data;
    }

    public static ModuleVersion.Fields MirroredStructMethodCall()
    {
        var version = new ModuleVersion(0, 1, 2);
        version.BumpRevision();
        return version.Data;
    }

    public static ModuleVersion.Fields MirroredStructInArguments()
    {
        var v1 = new ModuleVersion(0, 1, 2);
        var v2 = new ModuleVersion(0, 10, 20);
        var v3 = new ModuleVersion(0, 100, 200);
        return v1.SumOfSelfAnd(v2, v3).Data;
    }

    public static void SetCounterName([MarshalAs(UnmanagedType.LPStr)] string name)
    {
    }

    public static uint ProxyObjectNativeToManaged()
    {
        return 0;
    }
}