using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IInteractiveWindow
  {
    void Show(DockingState state);

    void Attach(IInteractiveWindowContainer targetWindow);
  }
}
