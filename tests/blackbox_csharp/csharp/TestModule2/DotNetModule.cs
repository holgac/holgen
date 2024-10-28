using System.Runtime.InteropServices;
using CSharpBindings;

namespace TestModule2;

public sealed class DotNetModule : IDotNetModule
{
    private uint _bumpAmount = 1;
    private static DotNetModule _instance = null!;

    public static void Initialize()
    {
        Console.WriteLine("Hello from TestModule2!");
        _instance = new DotNetModule();
    }

    public static ModuleVersion.Fields Constructor()
    {
        return new ModuleVersion(30, 40, 50).Data;
    }

    public static ModuleVersion.Fields MirroredStructManagedToNative()
    {
        ModuleVersion.Fields a;
        a.Major = 3;
        a.Minor = 4;
        a.Revision = 5;
        return a;
    }

    public static ModuleVersion.Fields MirroredStructNativeToManagedToNative()
    {
        return ModuleVersion.ConstructDouble(3, 4, 5).Data;
    }

    public static ModuleVersion.Fields MirroredStructMethodCall()
    {
        var version = new ModuleVersion(3, 4, 5);
        version.Data.Revision = version.GetSum();
        return version.Data;
    }

    public static ModuleVersion.Fields MirroredStructInArguments()
    {
        var v1 = new ModuleVersion(3, 4, 5);
        var v2 = new ModuleVersion(30, 40, 50);
        var v3 = new ModuleVersion(300, 400, 500);
        return v1.SumOfSelfAnd(v2, v3).Data;
    }


    public static uint BumpAndGetCounter()
    {
        SingletonCounter.Bump(_instance._bumpAmount * 2);
        return SingletonCounter.Get();
    }

    public static void SetBumpAmount(uint amount)
    {
        _instance._bumpAmount = amount;
    }

    public static void SetCounterName([MarshalAs(UnmanagedType.LPStr)] string name)
    {
    }

    public static uint ProxyObjectNativeToManaged()
    {
        return 0;
    }
}