namespace CSharpBindings;

using System.Runtime.InteropServices;

public class ModuleVersion
{
  public ModuleVersion(ushort major, ushort minor, uint revision)
  {
    Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionConstructDelegate>(_constructImpl)(major, minor, revision);
  }
  public ModuleVersion()
  {
  }
  
  [StructLayout(LayoutKind.Sequential)]
  public struct Fields
  {
    public ushort Major;
    public ushort Minor;
    public uint Revision;
  }
  
  public Fields Data;
  public ushort Major => Data.Major;
  public ushort Minor => Data.Minor;
  public uint Revision => Data.Revision;
  
  public static ModuleVersion Construct(ushort major, ushort minor, uint revision)
  {
    return new ModuleVersion
    {
      Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionConstructDelegate>(_constructImpl)(major, minor, revision)
    };
  }
  public static ModuleVersion ConstructDouble(ushort major, ushort minor, uint revision)
  {
    return new ModuleVersion
    {
      Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionConstructDoubleDelegate>(_constructDoubleImpl)(major, minor, revision)
    };
  }
  public uint GetSum()
  {
    return Marshal.GetDelegateForFunctionPointer<ModuleVersionGetSumDelegate>(_getSumImpl)(ref this.Data);
  }
  public void BumpRevision()
  {
    Marshal.GetDelegateForFunctionPointer<ModuleVersionBumpRevisionDelegate>(_bumpRevisionImpl)(ref this.Data);
  }
  public ModuleVersion SumOfSelfAnd(ModuleVersion v1, ModuleVersion v2)
  {
    return new ModuleVersion
    {
      Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionSumOfSelfAndDelegate>(_sumOfSelfAndImpl)(ref this.Data, ref v1.Data, ref v2.Data)
    };
  }
  public ModuleVersion GetNextRevisions(ulong count, ulong[] holgenReturnValueHolgenSize, DeferredDeleter holgenDeferredDeleter)
  {
    return new ModuleVersion
    {
      Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionGetNextRevisionsDelegate>(_getNextRevisionsImpl)(ref this.Data, count, holgenReturnValueHolgenSize, holgenDeferredDeleter.HolgenPtr)
    };
  }
  public ModuleVersion GetNextThreeRevisions(DeferredDeleter holgenDeferredDeleter)
  {
    return new ModuleVersion
    {
      Data = Marshal.GetDelegateForFunctionPointer<ModuleVersionGetNextThreeRevisionsDelegate>(_getNextThreeRevisionsImpl)(ref this.Data, holgenDeferredDeleter.HolgenPtr)
    };
  }
  
  public delegate ModuleVersion.Fields ModuleVersionConstructDelegate(ushort major, ushort minor, uint revision);
  public delegate ModuleVersion.Fields ModuleVersionConstructDoubleDelegate(ushort major, ushort minor, uint revision);
  public delegate uint ModuleVersionGetSumDelegate(ref ModuleVersion.Fields holgenObject);
  public delegate void ModuleVersionBumpRevisionDelegate(ref ModuleVersion.Fields holgenObject);
  public delegate ModuleVersion.Fields ModuleVersionSumOfSelfAndDelegate(ref ModuleVersion.Fields holgenObject, ref ModuleVersion.Fields v1, ref ModuleVersion.Fields v2);
  public delegate ModuleVersion.Fields ModuleVersionGetNextRevisionsDelegate(ref ModuleVersion.Fields holgenObject, ulong count, ulong[] holgenReturnValueHolgenSize, IntPtr holgenDeferredDeleter);
  public delegate ModuleVersion.Fields ModuleVersionGetNextThreeRevisionsDelegate(ref ModuleVersion.Fields holgenObject, IntPtr holgenDeferredDeleter);
  
  private static IntPtr _constructImpl = IntPtr.Zero;
  private static IntPtr _constructDoubleImpl = IntPtr.Zero;
  private static IntPtr _getSumImpl = IntPtr.Zero;
  private static IntPtr _bumpRevisionImpl = IntPtr.Zero;
  private static IntPtr _sumOfSelfAndImpl = IntPtr.Zero;
  private static IntPtr _getNextRevisionsImpl = IntPtr.Zero;
  private static IntPtr _getNextThreeRevisionsImpl = IntPtr.Zero;
  
  public delegate void ModuleVersionHolgenInitializeDelegate(ModuleVersionConstructDelegate construct, ModuleVersionConstructDoubleDelegate constructDouble, ModuleVersionGetSumDelegate getSum, ModuleVersionBumpRevisionDelegate bumpRevision, ModuleVersionSumOfSelfAndDelegate sumOfSelfAnd, ModuleVersionGetNextRevisionsDelegate getNextRevisions, ModuleVersionGetNextThreeRevisionsDelegate getNextThreeRevisions);
  
  public static void HolgenInitialize(ModuleVersionConstructDelegate construct, ModuleVersionConstructDoubleDelegate constructDouble, ModuleVersionGetSumDelegate getSum, ModuleVersionBumpRevisionDelegate bumpRevision, ModuleVersionSumOfSelfAndDelegate sumOfSelfAnd, ModuleVersionGetNextRevisionsDelegate getNextRevisions, ModuleVersionGetNextThreeRevisionsDelegate getNextThreeRevisions)
  {
    _constructImpl = Marshal.GetFunctionPointerForDelegate(construct);
    _constructDoubleImpl = Marshal.GetFunctionPointerForDelegate(constructDouble);
    _getSumImpl = Marshal.GetFunctionPointerForDelegate(getSum);
    _bumpRevisionImpl = Marshal.GetFunctionPointerForDelegate(bumpRevision);
    _sumOfSelfAndImpl = Marshal.GetFunctionPointerForDelegate(sumOfSelfAnd);
    _getNextRevisionsImpl = Marshal.GetFunctionPointerForDelegate(getNextRevisions);
    _getNextThreeRevisionsImpl = Marshal.GetFunctionPointerForDelegate(getNextThreeRevisions);
  }
}
