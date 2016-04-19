using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RVRMeander.Core.Window.Events
{
  public delegate void ComboItemsAddedEventHandler(object sender, ComboItemsAddedEvent e);

  public class ComboItemsAddedEvent : EventArgs
  {
    public ComboItemsAddedEvent()
    {
    }
  }
}
