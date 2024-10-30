using CSharpBindings;

namespace TestModule;

public class CounterBumperImpl : ICounterBumper
{
    public string Name = "Counter";
    
    public override void SetName(string name)
    {
        Name = name;
    }

    public override void Bump()
    {
        CounterManager.GetCounter(Name).Bump(1);
    }
}