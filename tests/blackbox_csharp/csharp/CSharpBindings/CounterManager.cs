namespace CSharpBindings;

using System.Runtime.InteropServices;

public class CounterManager
{
  public static Counter GetCounter([MarshalAs(UnmanagedType.LPStr)] string name)
  {
    return new Counter(Marshal.GetDelegateForFunctionPointer<CounterManagerGetCounterDelegate>(_getCounterImpl)(name));
  }
  public static Counter GetCounterPtr([MarshalAs(UnmanagedType.LPStr)] string name)
  {
    return new Counter(Marshal.GetDelegateForFunctionPointer<CounterManagerGetCounterPtrDelegate>(_getCounterPtrImpl)(name));
  }
  
  public delegate IntPtr CounterManagerGetCounterDelegate([MarshalAs(UnmanagedType.LPStr)] string name);
  public delegate IntPtr CounterManagerGetCounterPtrDelegate([MarshalAs(UnmanagedType.LPStr)] string name);
  
  private static IntPtr _getCounterImpl = IntPtr.Zero;
  private static IntPtr _getCounterPtrImpl = IntPtr.Zero;
  
  public delegate void CounterManagerHolgenInitializeDelegate(CounterManagerGetCounterDelegate getCounter, CounterManagerGetCounterPtrDelegate getCounterPtr);
  
  public static void HolgenInitialize(CounterManagerGetCounterDelegate getCounter, CounterManagerGetCounterPtrDelegate getCounterPtr)
  {
    _getCounterImpl = Marshal.GetFunctionPointerForDelegate(getCounter);
    _getCounterPtrImpl = Marshal.GetFunctionPointerForDelegate(getCounterPtr);
  }
}
