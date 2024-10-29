namespace CSharpBindings;

using System.Runtime.InteropServices;

public class SingletonCounter
{
  public static uint Get()
  {
    return Marshal.GetDelegateForFunctionPointer<SingletonCounterGetDelegate>(_getImpl)();
  }
  public static void Bump(uint amount)
  {
    Marshal.GetDelegateForFunctionPointer<SingletonCounterBumpDelegate>(_bumpImpl)(amount);
  }
  public static void Reset()
  {
    Marshal.GetDelegateForFunctionPointer<SingletonCounterResetDelegate>(_resetImpl)();
  }
  
  public delegate uint SingletonCounterGetDelegate();
  public delegate void SingletonCounterBumpDelegate(uint amount);
  public delegate void SingletonCounterResetDelegate();
  
  private static IntPtr _getImpl = IntPtr.Zero;
  private static IntPtr _bumpImpl = IntPtr.Zero;
  private static IntPtr _resetImpl = IntPtr.Zero;
  
  public delegate void SingletonCounterHolgenInitializeDelegate(SingletonCounterGetDelegate get, SingletonCounterBumpDelegate bump, SingletonCounterResetDelegate reset);
  
  public static void HolgenInitialize(SingletonCounterGetDelegate get, SingletonCounterBumpDelegate bump, SingletonCounterResetDelegate reset)
  {
    _getImpl = Marshal.GetFunctionPointerForDelegate(get);
    _bumpImpl = Marshal.GetFunctionPointerForDelegate(bump);
    _resetImpl = Marshal.GetFunctionPointerForDelegate(reset);
  }
}
