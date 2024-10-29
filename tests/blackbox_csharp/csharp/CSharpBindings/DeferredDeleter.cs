namespace CSharpBindings;

using System.Runtime.InteropServices;

public class DeferredDeleter
{
  public DeferredDeleter(IntPtr ptr)
  {
    HolgenPtr = ptr;
  }
  
  public IntPtr HolgenPtr { get; }
  
  public static void Perform(DeferredDeleter ptr)
  {
    Marshal.GetDelegateForFunctionPointer<DeferredDeleterPerformDelegate>(_performImpl)(ptr.HolgenPtr);
  }
  
  public delegate void DeferredDeleterPerformDelegate(IntPtr ptr);
  
  private static IntPtr _performImpl = IntPtr.Zero;
  
  public delegate void DeferredDeleterHolgenInitializeDelegate(DeferredDeleterPerformDelegate perform);
  
  public static void HolgenInitialize(DeferredDeleterPerformDelegate perform)
  {
    _performImpl = Marshal.GetFunctionPointerForDelegate(perform);
  }
}
