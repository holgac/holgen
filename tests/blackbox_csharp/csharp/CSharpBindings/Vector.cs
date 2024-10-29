namespace CSharpBindings;

using System.Runtime.InteropServices;

public class Vector
{
  public Vector(IntPtr ptr)
  {
    HolgenPtr = ptr;
  }
  
  public IntPtr HolgenPtr { get; }
  
  public delegate void VectorHolgenInitializeDelegate();
  
  public static void HolgenInitialize()
  {
  }
}
