using System;
using System.Collections.Generic;
using System.ComponentModel.Composition.Hosting;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Utils
{
  public class Mef
  {
    public static System.ComponentModel.Composition.Hosting.AggregateCatalog GetMefCatalogs()
    {
      var catalog = new AggregateCatalog();
      //catalog.Catalogs.Add(new DirectoryCatalog(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory), "*.dll"));
      //catalog.Catalogs.Add(new DirectoryCatalog(System.IO.Path.GetDirectoryName(System.AppDomain.CurrentDomain.BaseDirectory), "*.exe"));
      foreach (var ass in System.AppDomain.CurrentDomain.GetAssemblies())
      {
        if (!ass.FullName.StartsWith("System", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("Microsoft", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("mscorlib", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("Accessibility", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("vshost32", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("DotSpatial", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("log4net", StringComparison.OrdinalIgnoreCase) &&
          !ass.FullName.StartsWith("WeifenLuo", StringComparison.OrdinalIgnoreCase))
        {
          Debug.WriteLine("Found assembly: " + ass.FullName);
          catalog.Catalogs.Add(new AssemblyCatalog(ass));
        }
      }
      return catalog;
    }
  }
}
