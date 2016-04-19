using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Model.Mc
{
  public static class DllAccess
  {
    public const string DLL_Name_x86 = "RVRMeander.dll";
    public const string DLL_Name_x64 = "RVRMeander.dll";

    public static string Name_RVRDLL_Run = "RVRDLL_Run";    
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_Run(string configFile, string logFile);
    
    public static string Name_RVRDLL_Init = "RVRDLL_Init";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_Init(string configFile, string logFile);
    
    public static string Name_RVRDLL_GetNumIters = "RVRDLL_GetNumIters";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_GetNumIters(int handle);
    
    public static string Name_RVRDLL_Step = "RVRDLL_Step";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_Step(int handle, int stepIdx);
    
    public static string Name_RVRDLL_Finish = "RVRDLL_Finish";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_Finish(int handle);

    public static string Name_RVRDLL_Run_libconfig = "RVRDLL_Run_libconfig";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int RVRDLL_Run_libconfig(string configFile, string logFile);

  }
}
