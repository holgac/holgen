using System.Runtime.InteropServices;

namespace CSharpBindings;

public interface IDotNetModule
{
  public static abstract void Initialize();
  public static abstract uint BumpAndGetSingletonCounter();
  public static abstract void SetBumpAmount(uint amount);
  public static abstract ModuleVersion.Fields Constructor();
  public static abstract ModuleVersion.Fields MirroredStructManagedToNative();
  public static abstract ModuleVersion.Fields MirroredStructNativeToManagedToNative();
  public static abstract ModuleVersion.Fields MirroredStructMethodCall();
  public static abstract ModuleVersion.Fields MirroredStructInArguments();
  public static abstract void SetCounterName([MarshalAs(UnmanagedType.LPStr)] string name);
  public static abstract uint ProxyObjectNativeToManaged();
  public static abstract uint ProxyObjectMethodArgInModule(IntPtr counter);
  public static abstract IntPtr ProxyObjectReturnValue(IntPtr counter1, IntPtr counter2);
  public static abstract uint PrimitiveArrays();
  public static abstract void StringArraysNativeToManaged([MarshalAs(UnmanagedType.LPArray, ArraySubType=UnmanagedType.LPStr, SizeParamIndex=1)] string[] args, ulong argsHolgenSize);
  public static abstract void StringArraysManagedToNative();
  
  public delegate void DotNetModuleInitializeDelegate();
  public delegate uint DotNetModuleBumpAndGetSingletonCounterDelegate();
  public delegate void DotNetModuleSetBumpAmountDelegate(uint amount);
  public delegate ModuleVersion.Fields DotNetModuleConstructorDelegate();
  public delegate ModuleVersion.Fields DotNetModuleMirroredStructManagedToNativeDelegate();
  public delegate ModuleVersion.Fields DotNetModuleMirroredStructNativeToManagedToNativeDelegate();
  public delegate ModuleVersion.Fields DotNetModuleMirroredStructMethodCallDelegate();
  public delegate ModuleVersion.Fields DotNetModuleMirroredStructInArgumentsDelegate();
  public delegate void DotNetModuleSetCounterNameDelegate([MarshalAs(UnmanagedType.LPStr)] string name);
  public delegate uint DotNetModuleProxyObjectNativeToManagedDelegate();
  public delegate uint DotNetModuleProxyObjectMethodArgInModuleDelegate(IntPtr counter);
  public delegate IntPtr DotNetModuleProxyObjectReturnValueDelegate(IntPtr counter1, IntPtr counter2);
  public delegate uint DotNetModulePrimitiveArraysDelegate();
  public delegate void DotNetModuleStringArraysNativeToManagedDelegate([MarshalAs(UnmanagedType.LPArray, ArraySubType=UnmanagedType.LPStr, SizeParamIndex=1)] string[] args, ulong argsHolgenSize);
  public delegate void DotNetModuleStringArraysManagedToNativeDelegate();
  
}
