using System.Runtime.InteropServices;
using System.Reflection;

namespace CSharpBindings;

public sealed class HolgenMain
{
  public static HolgenMain Instance = null!;
  public List<Assembly> Assemblies { get; } = [];
  
  public HolgenMain()
  {
  }
  
  public static void Main()
  {
  }
  
  public delegate void InitializeDelegate();
  public static void Initialize()
  {
    Instance = new HolgenMain();
    Console.WriteLine("Holgen C# initialization successful!");
  }
  
  public delegate void LoadModuleDelegate([MarshalAs(UnmanagedType.LPStr)] string path);
  public static void LoadModule([MarshalAs(UnmanagedType.LPStr)] string path)
  {
    Console.WriteLine("Loading module {0}", path);
    var asm = Assembly.LoadFrom(path);
    Instance.Assemblies.Add(asm);
  }
}
