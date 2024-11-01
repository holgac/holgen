using CSharpBindings;

namespace TestModule;

public class CounterBumperImpl : ICounterBumper
{
    public string Name = "Counter";

    public string Field
    {
        get { return "a"; }
        set { Name = value; }
    }

    public override void SetName(string name)
    {
        Name = name;
    }

    public override string GetName()
    {
        return Name;
    }

    public override void Bump()
    {
        CounterManager.GetCounter(Name).Bump(1);
    }

    public override void SetNameFromCounter(Counter ctr)
    {
        Name = $"Counter{ctr.Val}";
    }

    public override void SetNameFromRevision(ModuleVersion version)
    {
        Name = $"Counter{version.Revision}";
    }

    public override ModuleVersion GetVersion()
    {
        return new ModuleVersion(12, 13, 14);
    }

    public override Counter GetCounter()
    {
        return CounterManager.GetCounter(Name);
    }

    public override ICounterBumper Clone()
    {
        return new CounterBumperImpl
        {
            Name = Name
        };
    }

    public override void CopyFrom(ICounterBumper other)
    {
        var impl = (CounterBumperImpl)other;
        Console.WriteLine($"Copy from {impl.Name}, was {Name} before");
        Name = impl.Name;
    }

    public override void BumpMultiple(string[] names, uint[] counts)
    {
        for (int i = 0; i < names.Length; i++)
        {
            var c = CounterManager.GetCounter(names[i]);
            c.Bump(counts[i]);
        }
    }

    public override void SetNameConcatArray(string[] name)
    {
        Name = string.Join("", name);
    }

    public override int[] SplitNameAndParseSigned(sbyte delim)
    {
        var split = Name.Split((char)delim);
        var result = new int[split.Length];
        for (int i = 0; i < split.Length; i++)
        {
            result[i] = int.Parse(split[i]);
        }

        return result;
    }

    public override string[] SplitName(sbyte delim)
    {
        return Name.Split((char)delim);
    }

    public override ICounterBumper[] CloneMultiple(ulong count)
    {
        var res = new CounterBumperImpl[count];
        for (int i = 0; i < res.Length; i++)
        {
            res[i] = new CounterBumperImpl();
            res[i].Name = $"{Name}{i}";
        }
        return res;
    }

    //
    // public override uint[] SplitNameAndParse(sbyte delim)
    // {
    //     var split = Name.Split((char)delim);
    //     var result = new uint[split.Length];
    //     for (int i = 0; i < split.Length; i++)
    //     {
    //         result[i] = uint.Parse(split[i]);
    //     }
    //
    //     return result;
    // }
    //
    // public override string[] SplitNameIntoThree()
    // {
    //     var l = Name.Length / 3;
    //     return [Name.Substring(0, l), Name.Substring(l, l * 2), Name.Substring(l + 2)];
    // }
}