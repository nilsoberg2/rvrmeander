using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window.Events
{
  /// <summary>
  /// This class allows classes to interact with the main window container, instructing it to change
  /// the button status in response to events within other assemblies.
  /// </summary>
  public class ToolbarItemStatusChanged
  {
    public bool Enabled { get; set; }

    public IToolbarItem Item { get; set; }

    public ToolbarItemStatusChanged(IToolbarItem item, bool enabled)
    {
      Item = item;
      Enabled = enabled;
    }
  }
}
