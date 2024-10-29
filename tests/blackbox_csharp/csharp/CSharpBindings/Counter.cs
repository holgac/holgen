namespace CSharpBindings;

using System.Runtime.InteropServices;

public class Counter
{
  public Counter(IntPtr ptr)
  {
    HolgenPtr = ptr;
  }
  
  public IntPtr HolgenPtr { get; }
  
  public uint Get()
  {
    return Marshal.GetDelegateForFunctionPointer<CounterGetDelegate>(_getImpl)(this.HolgenPtr);
  }
  public void Bump(uint amount)
  {
    Marshal.GetDelegateForFunctionPointer<CounterBumpDelegate>(_bumpImpl)(this.HolgenPtr, amount);
  }
  public void Reset()
  {
    Marshal.GetDelegateForFunctionPointer<CounterResetDelegate>(_resetImpl)(this.HolgenPtr);
  }
  public Counter AddCounterAndReturnSelf(Counter rhs)
  {
    return new Counter(Marshal.GetDelegateForFunctionPointer<CounterAddCounterAndReturnSelfDelegate>(_addCounterAndReturnSelfImpl)(this.HolgenPtr, rhs.HolgenPtr));
  }
  public void BumpMultiple([MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2)] uint[] amounts)
  {
    Marshal.GetDelegateForFunctionPointer<CounterBumpMultipleDelegate>(_bumpMultipleImpl)(this.HolgenPtr, amounts, (ulong)amounts.Length);
  }
  public void BumpMultipleCopy([MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2)] uint[] amounts)
  {
    Marshal.GetDelegateForFunctionPointer<CounterBumpMultipleCopyDelegate>(_bumpMultipleCopyImpl)(this.HolgenPtr, amounts, (ulong)amounts.Length);
  }
  public void BumpByStrlensOf([MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2, ArraySubType=UnmanagedType.LPStr)] string[] words)
  {
    Marshal.GetDelegateForFunctionPointer<CounterBumpByStrlensOfDelegate>(_bumpByStrlensOfImpl)(this.HolgenPtr, words, (ulong)words.Length);
  }
  public void BumpByStrlensOfCopy([MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2, ArraySubType=UnmanagedType.LPStr)] string[] words)
  {
    Marshal.GetDelegateForFunctionPointer<CounterBumpByStrlensOfCopyDelegate>(_bumpByStrlensOfCopyImpl)(this.HolgenPtr, words, (ulong)words.Length);
  }
  
  public delegate uint CounterGetDelegate(IntPtr holgenObject);
  public delegate void CounterBumpDelegate(IntPtr holgenObject, uint amount);
  public delegate void CounterResetDelegate(IntPtr holgenObject);
  public delegate IntPtr CounterAddCounterAndReturnSelfDelegate(IntPtr holgenObject, IntPtr rhs);
  public delegate void CounterBumpMultipleDelegate(IntPtr holgenObject, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2)] uint[] amounts, ulong amountsHolgenSize);
  public delegate void CounterBumpMultipleCopyDelegate(IntPtr holgenObject, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2)] uint[] amounts, ulong amountsHolgenSize);
  public delegate void CounterBumpByStrlensOfDelegate(IntPtr holgenObject, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2, ArraySubType=UnmanagedType.LPStr)] string[] words, ulong wordsHolgenSize);
  public delegate void CounterBumpByStrlensOfCopyDelegate(IntPtr holgenObject, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex=2, ArraySubType=UnmanagedType.LPStr)] string[] words, ulong wordsHolgenSize);
  
  private static IntPtr _getImpl = IntPtr.Zero;
  private static IntPtr _bumpImpl = IntPtr.Zero;
  private static IntPtr _resetImpl = IntPtr.Zero;
  private static IntPtr _addCounterAndReturnSelfImpl = IntPtr.Zero;
  private static IntPtr _bumpMultipleImpl = IntPtr.Zero;
  private static IntPtr _bumpMultipleCopyImpl = IntPtr.Zero;
  private static IntPtr _bumpByStrlensOfImpl = IntPtr.Zero;
  private static IntPtr _bumpByStrlensOfCopyImpl = IntPtr.Zero;
  
  public delegate void CounterHolgenInitializeDelegate(CounterGetDelegate get, CounterBumpDelegate bump, CounterResetDelegate reset, CounterAddCounterAndReturnSelfDelegate addCounterAndReturnSelf, CounterBumpMultipleDelegate bumpMultiple, CounterBumpMultipleCopyDelegate bumpMultipleCopy, CounterBumpByStrlensOfDelegate bumpByStrlensOf, CounterBumpByStrlensOfCopyDelegate bumpByStrlensOfCopy);
  
  public static void HolgenInitialize(CounterGetDelegate get, CounterBumpDelegate bump, CounterResetDelegate reset, CounterAddCounterAndReturnSelfDelegate addCounterAndReturnSelf, CounterBumpMultipleDelegate bumpMultiple, CounterBumpMultipleCopyDelegate bumpMultipleCopy, CounterBumpByStrlensOfDelegate bumpByStrlensOf, CounterBumpByStrlensOfCopyDelegate bumpByStrlensOfCopy)
  {
    _getImpl = Marshal.GetFunctionPointerForDelegate(get);
    _bumpImpl = Marshal.GetFunctionPointerForDelegate(bump);
    _resetImpl = Marshal.GetFunctionPointerForDelegate(reset);
    _addCounterAndReturnSelfImpl = Marshal.GetFunctionPointerForDelegate(addCounterAndReturnSelf);
    _bumpMultipleImpl = Marshal.GetFunctionPointerForDelegate(bumpMultiple);
    _bumpMultipleCopyImpl = Marshal.GetFunctionPointerForDelegate(bumpMultipleCopy);
    _bumpByStrlensOfImpl = Marshal.GetFunctionPointerForDelegate(bumpByStrlensOf);
    _bumpByStrlensOfCopyImpl = Marshal.GetFunctionPointerForDelegate(bumpByStrlensOfCopy);
  }
}
