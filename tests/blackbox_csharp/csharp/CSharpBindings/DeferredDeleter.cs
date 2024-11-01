// This file is generated by holgen. Do not modify manually.

using System.Runtime.InteropServices;

namespace CSharpBindings;

internal static class DeferredDeleter
{
  public static void Perform(IntPtr rawPtr)
  {
    Marshal.GetDelegateForFunctionPointer<DeferredDeleterPerformDelegate>(_performImpl)(rawPtr);
  }
  public static void HolgenInitialize(DeferredDeleterPerformDelegate deferredDeleterPerformDelegate)
  {
    _performImpl = Marshal.GetFunctionPointerForDelegate(deferredDeleterPerformDelegate);
  }
  public static void PerformManaged(IntPtr rawPtr)
  {
    Marshal.FreeHGlobal(rawPtr);
  }
  public static void PerformManagedArray(IntPtr rawPtr, ulong count)
  {
    for (int i = 0; i < (int)count; ++i)
    {
      var data = Marshal.ReadIntPtr(rawPtr, i * IntPtr.Size);
      Marshal.FreeHGlobal(data);
    }
    Marshal.FreeHGlobal(rawPtr);
  }
  
  public delegate void DeferredDeleterPerformDelegate(IntPtr rawPtr);
  public delegate void DeferredDeleterHolgenInitializeDelegate(DeferredDeleterPerformDelegate deferredDeleterPerformDelegate);
  public delegate void DeferredDeleterPerformManagedDelegate(IntPtr rawPtr);
  public delegate void DeferredDeleterPerformManagedArrayDelegate(IntPtr rawPtr, ulong count);
  
  private static IntPtr _performImpl = IntPtr.Zero;
  
}
