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
}