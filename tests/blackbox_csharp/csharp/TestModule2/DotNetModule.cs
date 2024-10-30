using System.Runtime.InteropServices;
using CSharpBindings;

namespace TestModule2;

public sealed class DotNetModule : IDotNetModule
{
    private uint _bumpAmount = 1;
    private string _counterName = "TestModule2";
    private static DotNetModule _instance = null!;

    public static void Initialize()
    {
        Console.WriteLine("Hello from TestModule2!");
        _instance = new DotNetModule();
    }

    public static uint BumpAndGetSingletonCounter()
    {
        SingletonCounter.Bump(_instance._bumpAmount * 2);
        return SingletonCounter.Get();
    }

    public static void SetBumpAmount(uint amount)
    {
        _instance._bumpAmount = amount;
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

    public static void SetCounterName([MarshalAs(UnmanagedType.LPStr)] string name)
    {
        Console.WriteLine("TestModule2 set counter name to {0}", name);
        _instance._counterName = name;
    }

    public static uint ProxyObjectNativeToManaged()
    {
        var counter = CounterManager.GetCounterPtr(_instance._counterName);
        counter.Bump(_instance._bumpAmount * 2);
        return counter.Get();
    }

    public static uint ProxyObjectMethodArgInModule(IntPtr counter)
    {
        var counterInstance = new Counter(counter);
        return counterInstance.Get() * 2;
    }

    public static IntPtr ProxyObjectReturnValue(IntPtr counter1, IntPtr counter2)
    {
        var c1 = new Counter(counter1);
        var c2 = new Counter(counter2);
        var c1Again = c1.AddCounterAndReturnSelf(c2);
        return c1Again.AddCounterAndReturnSelf(c2).HolgenPtr;
    }

    public static void ProxyObjectField()
    {
        throw new NotImplementedException();
    }

    public static uint PrimitiveArrays()
    {
        var c = CounterManager.GetCounter(_instance._counterName);
        c.BumpMultipleCopy([3, 4, 5]);
        return c.Get();
    }

    public static void StringArraysNativeToManaged(string[] args, ulong argsHolgenCount)
    {
        foreach (var arg in args)
        {
            var c = CounterManager.GetCounter(arg);
            c.Bump(_instance._bumpAmount * 2);
        }
    }
    public static void StringArraysManagedToNative()
    {
        var c = CounterManager.GetCounter(_instance._counterName);
        c.BumpByStrlensOfCopy(["hello", "how", "are", "you"]);
    }

    public static uint MirroredStructReturnVector()
    {
        throw new NotImplementedException();
    }

    public static uint MirroredStructReturnArray()
    {
        throw new NotImplementedException();
    }

    public static int PrimitiveReturnVector()
    {
        throw new NotImplementedException();
    }

    public static uint PrimitiveReturnArray()
    {
        throw new NotImplementedException();
    }

    public static void TrackedCSharpObject(uint addCount)
    {
        throw new NotImplementedException();
    }
}