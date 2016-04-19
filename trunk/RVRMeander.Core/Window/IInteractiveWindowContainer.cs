using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IInteractiveWindowContainer
  {
    void Show(IInteractiveWindow dockableWindow, DockingState state);

    WindowOperation StartOperation();

    void StopOperation();

    void SetProgress(int percentage);

    void ResetProgress();
  }
}
