using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Gui.Util
{
  class AppData
  {
    public static string GetApplicationDirectory()
    {
      string appDirPath = "";
      //#if __MonoCS__
      //#else
      //      // If we are deployed via click-once, then modify the log file path to be the data directory
      //      if (!System.Deployment.Application.ApplicationDeployment.IsNetworkDeployed)
      //#endif
      //      {
      //        if (IsRunningOnMono())
      //        {
      //          appDirPath = System.AppDomain.CurrentDomain.BaseDirectory;
      //        }
      //        else
      //        {
      string appDir = Path.Combine(Environment.GetEnvironmentVariable("LOCALAPPDATA"), "UniversityOfIllinois", "RVRMeander", "2.0");
      bool ok = true;
      if (!Directory.Exists(appDir))
      {
        try
        {
          Directory.CreateDirectory(appDir);
        }
        catch
        {
          ok = false;
        }
      }
      if (ok)
      {
        appDirPath = appDir;
      }
      //        }
      //      }
      //#if __MonoCS__
      //#else
      //      else
      //      {
      //var dataDir = System.Deployment.Application.ApplicationDeployment.CurrentDeployment.DataDirectory;
      //appDirPath = dataDir;
      //      }
      //#endif

      return appDirPath;
    }

    public static bool IsRunningOnMono()
    {
      return Type.GetType("Mono.Runtime") != null;
    }
  }
}
