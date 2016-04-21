using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Project.Events
{
  public class PackageOpened
  {
    public string PackagePath { get; set; }

    public string Projection { get; set; }
  }
}
