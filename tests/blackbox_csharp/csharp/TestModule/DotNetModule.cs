using System.Runtime.InteropServices;
using CSharpBindings;

namespace TestModule;

public sealed class DotNetModule : IDotNetModule
{
    private uint _bumpAmount = 1;

    private string _counterName = "TestModule";
    private static DotNetModule _instance = null!;

    public static void Initialize()
    {
        Console.WriteLine("Hello from TestModule!");
        _instance = new DotNetModule();
    }

    public static uint BumpAndGetSingletonCounter()
    {
        SingletonCounter.Bump(_instance._bumpAmount);
        return SingletonCounter.Get();
    }

    public static void SetBumpAmount(uint amount)
    {
        _instance._bumpAmount = amount;
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
        Console.WriteLine("TestModule set counter name to {0}", name);
        _instance._counterName = name;
    }

    public static uint ProxyObjectNativeToManaged()
    {
        var counter = CounterManager.GetCounter(_instance._counterName);
        counter.Bump(_instance._bumpAmount);
        return counter.Get();
    }

    public static uint ProxyObjectMethodArgInModule(IntPtr counter)
    {
        var counterInstance = new Counter(counter);
        return counterInstance.Get();
    }

    public static IntPtr ProxyObjectReturnValue(IntPtr counter1, IntPtr counter2)
    {
        var c1 = new Counter(counter1);
        var c2 = new Counter(counter2);
        return c1.AddCounterAndReturnSelf(c2).HolgenPtr;
    }

    public static uint PrimitiveArrays()
    {
        var c = CounterManager.GetCounter(_instance._counterName);
        c.BumpMultiple([0, 1, 2]);
        return c.Get();
    }

    public static void StringArraysNativeToManaged(string[] args, ulong argsHolgenCount)
    {
        foreach (var arg in args)
        {
            var c = CounterManager.GetCounter(arg);
            c.Bump(_instance._bumpAmount);
        }
    }

    public static void StringArraysManagedToNative()
    {
        var c = CounterManager.GetCounter(_instance._counterName);
        c.BumpByStrlensOf(["hello", "how", "are", "you"]);
    }
}