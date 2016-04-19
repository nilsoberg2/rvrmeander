using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IStatusBar : RVRMeander.Progress.IProgressBar
  {
    void SetMessage(string message);
  }
}
