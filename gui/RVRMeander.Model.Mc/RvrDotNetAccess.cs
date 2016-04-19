using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  class RvrDotNetAccess
  {
    private DllAccess.RVRDLL_Run dllRun;
    private DllAccess.RVRDLL_Init dllInit;
    private DllAccess.RVRDLL_GetNumIters dllGetNumIters;
    private DllAccess.RVRDLL_Step dllStep;
    private DllAccess.RVRDLL_Finish dllFinish;
    
    IntPtr dllHandle;
    private bool is32bit;
    private int rvrHandle;

    public string ErrorMessage { get; set; }

    public RvrDotNetAccess()
    {
      this.is32bit = (IntPtr.Size != 8);
      this.dllHandle = IntPtr.Zero;
    }

    ~RvrDotNetAccess()
    {
      Dispose(false);
    }

    public void Dispose()
    {
      Dispose(true);
      GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposeCalledByClient)
    {
      FreeDll();

      // Managed objects
      if (disposeCalledByClient)
      {
      }
    }

    private void FreeDll()
    {
      // Unmanaged objects
      // Free the DLL library and delete the temporary copy.
      if (this.dllHandle != IntPtr.Zero)
      {
        try
        {
          PInvoke.FreeLibrary(this.dllHandle);
          this.dllHandle = IntPtr.Zero;
        }
        catch (Exception ex)
        {
          // Ignore the exception at this point because errors here are not system-critical.
        }
      }
    }

    /// <summary>
    /// Map functions in the library to delegates in this class.
    /// </summary>
    /// <param name="h">library handle</param>
    private void MapFunctions(IntPtr h)
    {
      IntPtr pFunc;
  
      pFunc = PInvoke.GetProcAddress(h, DllAccess.Name_RVRDLL_Run);
      this.dllRun = (DllAccess.RVRDLL_Run)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllAccess.RVRDLL_Run));
      
      pFunc = PInvoke.GetProcAddress(h, DllAccess.Name_RVRDLL_Init);
      this.dllInit = (DllAccess.RVRDLL_Init)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllAccess.RVRDLL_Init));
      
      pFunc = PInvoke.GetProcAddress(h, DllAccess.Name_RVRDLL_GetNumIters);
      this.dllGetNumIters = (DllAccess.RVRDLL_GetNumIters)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllAccess.RVRDLL_GetNumIters));
      
      pFunc = PInvoke.GetProcAddress(h, DllAccess.Name_RVRDLL_Step);
      this.dllStep = (DllAccess.RVRDLL_Step)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllAccess.RVRDLL_Step));
      
      pFunc = PInvoke.GetProcAddress(h, DllAccess.Name_RVRDLL_Finish);
      this.dllFinish = (DllAccess.RVRDLL_Finish)Marshal.GetDelegateForFunctionPointer(pFunc, typeof(DllAccess.RVRDLL_Finish));
    }

    private string GetDllName()
    {
      string dllName = DllAccess.DLL_Name_x86;
      if (!this.is32bit)
      {
        dllName = DllAccess.DLL_Name_x64;
      }
      return dllName;
    }

    public static string AssemblyDirectory
    {
      get
      {
        string codeBase = Assembly.GetExecutingAssembly().CodeBase;
        UriBuilder uri = new UriBuilder(codeBase);
        string path = Uri.UnescapeDataString(uri.Path);
        return Path.GetDirectoryName(path);
      }
    }

    /// <summary>
    /// Loads the DLL and initializes the RVR computational engine.
    /// </summary>
    /// <param name="iniConfigFilePath"></param>
    /// <param name="logFilePath"></param>
    /// <returns></returns>
    public bool Initialize(string iniConfigFilePath, string logFilePath)
    {
      // Free the DLL if we have loaded one already
      FreeDll();
      string dllName = GetDllName();

      string platform = this.is32bit ? "x86" : "x64";
      string dllPath = Path.Combine(AssemblyDirectory, platform, dllName);

      try
      {
        this.dllHandle = PInvoke.LoadLibrary(dllPath);
      }
      catch (Exception ex)
      {
        this.ErrorMessage = "Unable to load library from " + dllPath + ": " + ex.Message;
        return false;
      }

      try
      {
        MapFunctions(this.dllHandle);
      }
      catch (Exception ex)
      {
        FreeDll();
        this.ErrorMessage = "Unable to map library functions: " + ex.Message;
        return false;
      }

      this.rvrHandle = this.dllInit(iniConfigFilePath, logFilePath);
      if (this.rvrHandle <= 0)
      {
        this.ErrorMessage = "Unable to initialize RVR Meander from input file " + iniConfigFilePath + " and log file " + logFilePath;
        return false;
      }

      this.ErrorMessage = "";

      return true;
    }

    /// <summary>
    /// Returns negative number if there was an error, otherwise the number of iterations specified by the configuration file.
    /// </summary>
    /// <returns></returns>
    public int GetNumberOfIterations()
    {
      if (this.rvrHandle > 0)
      {
        return this.dllGetNumIters(this.rvrHandle);
      }
      else
      {
        return -1;
      }
    }

    /// <summary>
    /// Executes the engine at the given iteration.
    /// </summary>
    /// <param name="iterationIndex"></param>
    /// <returns></returns>
    public bool ExecuteStep(int iterationIndex)
    {
      if (this.rvrHandle > 0)
      {
        if (this.dllStep(this.rvrHandle, iterationIndex) != 0)
        {
          return false;
        }
        else
        {
          return true;
        }
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Wraps up the simulation and closes files.
    /// </summary>
    public void Finish()
    {
      if (this.rvrHandle > 0)
      {
        this.dllFinish(this.rvrHandle);
      }
    }
  }
}
