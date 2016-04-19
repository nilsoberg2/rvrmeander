using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RVRMeander.Core.Window
{
  public class WindowOperation : IDisposable
  {
    private IInteractiveWindowContainer mainWindow;
    
    public WindowOperation(IInteractiveWindowContainer mainWindow)
    {
      this.mainWindow = mainWindow;
    }

    public void Dispose()
    {
      if (this.mainWindow != null)
      {
        this.mainWindow.StopOperation();
      }
    }
  }
}
