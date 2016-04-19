using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window
{
  public interface IHostedControlToolbarItem : IToolbarItem
  {
    System.Windows.Forms.UserControl UserControl { get; }

    /// <summary>
    /// Called when the user control has been completed, it is closed after this event.
    /// </summary>
    event EventHandler UserControlFinished;
  }
}
