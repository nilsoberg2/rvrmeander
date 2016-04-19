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
  static class PInvoke
  {

    [System.Runtime.InteropServices.DllImport("kernel32.dll")]
    public static extern bool FreeLibrary(IntPtr hModule);

    [System.Runtime.InteropServices.DllImport("kernel32.dll")]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

    [System.Runtime.InteropServices.DllImport("kernel32.dll")]
    public static extern IntPtr LoadLibrary(string lpFileName);

    /// <summary>
    /// Check if the process is a 64-bit or 32-bit process.  Does not check if the system OS is 64-bit or 32-bit, just the process.
    /// </summary>
    /// <returns></returns>
    static bool CheckIs64bitProcess()
    {
      if (IntPtr.Size == 8)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    /// <summary>
    /// Sets the environment variables so that SpatiaLite can find the dll's
    /// </summary>
    /// <returns>true if successful</returns>
    static string GetPlatformBinaryDirectory()
    {
      string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

      if (CheckIs64bitProcess())
      {
        appPath = Path.Combine(appPath, "x64");
      }
      else
      {
        appPath = Path.Combine(appPath, "x86");
      }

      return appPath;
    }

    /// <summary>
    /// Sets the environment variables so that .Net can find the native dll's
    /// </summary>
    /// <returns>true if successful</returns>
    public static bool SetEnvironmentVars()
    {
      string appPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
      string binFilesDir = "";

      try
      {
        binFilesDir = GetPlatformBinaryDirectory();

        StringBuilder sb = new StringBuilder(1024);
        GetDllDirectory(1024, sb);
        //log.Debug("GetDllDirectory={0}", sb.ToString());

        if (!sb.ToString().Contains(binFilesDir))
        {
          SetDllDirectory(binFilesDir);
          //log.Debug("SetDllDirectory={0}", binFilesDir);
        }
      }
      catch (Exception ex)
      {
        //log.Error("Unable to set application path for unmanaged DLL's: {0}", ex.Message);
        return false;
      }

      return true;
    }


    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    static extern bool SetDllDirectory(string lpPathName);

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    static extern int GetDllDirectory(int nBufferLength, StringBuilder lpPathName);

  }
}
